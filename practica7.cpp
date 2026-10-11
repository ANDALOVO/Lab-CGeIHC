/*
Práctica 7: Iluminación 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
// Añadir Texturas del dado y del holocron
Texture logosStarWars;
Texture holocronTexture;


Model Blackhawk_M;
Model Rover_M; //Sustituir por sus propios Rovers Con jerarquía
//Añadir modelo del avión.
//Model Avion_M;
Model Avion;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Cada faro es hijo de la matriz de su vehiculo.
void ActualizarFaro(SpotLight& faro, const glm::mat4& padre,
	const glm::vec3& posicionLocal, const glm::vec3& direccionLocal)
{
	// w=1 hereda la traslacion; w=0 transforma solo la direccion.
	glm::vec3 posicionMundo = glm::vec3(padre * glm::vec4(posicionLocal, 1.0f));
	glm::vec3 direccionMundo = glm::normalize(
		glm::vec3(padre * glm::vec4(direccionLocal, 0.0f)));
	faro.SetFlash(posicionMundo, direccionMundo);
}

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado() //sustituir con su propio dado, para que tenga la textura de la práctica 6
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};	
	GLfloat cubo_vertices[] = {

// front
//x		y		z		S		T			NX		NY		NZ
-0.5f, -0.5f,  0.5f,    0.0954f, 0.8235f,     0.0f,   0.0f,  -1.0f,  // Abajo-Izquierda
 0.5f, -0.5f,  0.5f,    0.2075f, 0.8235f,     0.0f,   0.0f,  -1.0f,  // Abajo-Derecha
 0.5f,  0.5f,  0.5f,    0.2075f, 0.9672f,     0.0f,   0.0f,  -1.0f,  // Arriba-Derecha
-0.5f,  0.5f,  0.5f,    0.0954f, 0.9672f,     0.0f,   0.0f,  -1.0f,  // Arriba-Izquierda

// right 
0.5f, -0.5f,  0.5f,	    0.2642f, 0.8235f,		-1.0f,	0.0f,	0.0f,
0.5f, -0.5f,  -0.5f,	0.3751f, 0.8235f,		-1.0f,	0.0f,	0.0f,
0.5f,  0.5f,  -0.5f,	0.3751f, 0.9672f,		-1.0f,	0.0f,	0.0f,
0.5f,  0.5f,  0.5f,	    0.2642f, 0.9672f,		-1.0f,	0.0f,	0.0f,

// back 
-0.5f, -0.5f, -0.5f,	0.4240f, 0.8235f,		0.0f,	0.0f,	1.0f,
0.5f, -0.5f, -0.5f,		0.5383f, 0.8235f,		0.0f,	0.0f,	1.0f,
0.5f,  0.5f, -0.5f,		0.5383f, 0.9672f,		0.0f,	0.0f,	1.0f,
-0.5f,  0.5f, -0.5f,	0.4240f, 0.9672f,		0.0f,	0.0f,	1.0f,

// left 
-0.5f, -0.5f,  -0.5f,	0.5905f, 0.8235f,		1.0f,	0.0f,	0.0f,
-0.5f, -0.5f,  0.5f,	0.6770f, 0.8235f,		1.0f,	0.0f,	0.0f,
-0.5f,  0.5f,  0.5f,	0.6770f, 0.9672f,		1.0f,	0.0f,	0.0f,
-0.5f,  0.5f,  -0.5f,	0.5905f, 0.9672f,		1.0f,	0.0f,	0.0f,

// bottom 
-0.5f, -0.5f,  0.5f,	0.7159f, 0.8235f,		0.0f,	1.0f,	0.0f,
0.5f,  -0.5f,  0.5f,	0.8302f, 0.8235f,		0.0f,	1.0f,	0.0f,
 0.5f,  -0.5f,  -0.5f,	0.8302f, 0.9672f,		0.0f,	1.0f,	0.0f,
-0.5f, -0.5f,  -0.5f,	0.7159f, 0.9672f,		0.0f,	1.0f,	0.0f,

//UP 
 -0.5f, 0.5f,  0.5f,	0.8602f, 0.8235f,		0.0f,	-1.0f,	0.0f,
 0.5f,  0.5f,  0.5f,	0.9523f, 0.8235f,		0.0f,	-1.0f,	0.0f,
  0.5f, 0.5f,  -0.5f,	0.9523f, 0.9672f,		0.0f,	-1.0f,	0.0f,
 -0.5f, 0.5f,  -0.5f,	0.8602f, 0.9672f,		0.0f,	-1.0f,	0.0f,
	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	GLfloat holocron_vertices[] = {
		// X, Y, Z, S, T, NX, NY, NZ
		// 1. Izquierda: 
		-2.52f, -2.58f, 0.00f, 0.09f, 0.82f, 1.00f, -0.01f, 0.04f, // 0
		-2.65f, -0.14f, 2.49f, 0.21f, 0.82f, 1.00f, -0.01f, 0.04f, // 1
		-2.45f, -0.06f, -2.48f, 0.09f, 0.97f, 1.00f, -0.01f, 0.04f, // 2
		-2.45f, 2.42f, 0.08f, 0.22f, 0.97f, 1.00f, -0.01f, 0.04f, // 3
		// 2. Superior: 
		0.05f, 2.44f, -2.42f, 0.38f, 0.98f, 0.01f, -1.00f, -0.01f, // 4
		0.05f, 2.40f, 2.55f, 0.26f, 0.82f, 0.01f, -1.00f, -0.01f, // 5
		2.52f, 2.45f, 0.06f, 0.38f, 0.82f, 0.01f, -1.00f, -0.01f, // 6
		-2.45f, 2.42f, 0.08f, 0.26f, 0.98f, 0.01f, -1.00f, -0.01f, // 7
		// 3. Posterior: 
		0.05f, 2.44f, -2.42f, 0.42f, 0.97f, 0.02f, 0.00f, 1.00f, // 8
		2.53f, -0.06f, -2.58f, 0.42f, 0.82f, 0.02f, 0.00f, 1.00f, // 9
		0.08f, -2.51f, -2.43f, 0.54f, 0.82f, 0.02f, 0.00f, 1.00f, // 10
		-2.45f, -0.06f, -2.48f, 0.54f, 0.97f, 0.02f, 0.00f, 1.00f, // 11
		// 4. Derecha:
		2.59f, -0.09f, 2.51f, 0.58f, 0.82f, -1.00f, -0.02f, 0.01f, // 12
		2.53f, -0.06f, -2.58f, 0.69f, 0.98f, -1.00f, -0.02f, 0.01f, // 13
		2.63f, -2.55f, 0.01f, 0.68f, 0.82f, -1.00f, -0.02f, 0.01f, // 14
		2.52f, 2.45f, 0.06f, 0.58f, 0.98f, -1.00f, -0.02f, 0.01f, // 15
		// 5. Frontal
		0.05f, 2.40f, 2.55f, 0.83f, 0.98f, 0.00f, 0.00f, -1.00f, // 16
		-2.65f, -0.14f, 2.49f, 0.71f, 0.97f, 0.00f, 0.00f, -1.00f, // 17
		2.59f, -0.09f, 2.51f, 0.84f, 0.82f, 0.00f, 0.00f, -1.00f, // 18
		-0.14f, -2.60f, 2.56f, 0.71f, 0.82f, 0.00f, 0.00f, -1.00f, // 19
		// 6. Inferior:
		-2.52f, -2.58f, 0.00f, 0.96f, 0.96f, -0.01f, 1.00f, 0.02f, // 20
		-0.14f, -2.60f, 2.56f, 0.95f, 0.82f, -0.01f, 1.00f, 0.02f, // 21
		0.08f, -2.51f,	-2.43f, 0.87f, 0.97f, -0.01f, 1.00f, 0.02f, // 22
		2.63f, -2.55f,	0.01f, 0.85f, 0.83f, -0.01f, 1.00f, 0.02f, // 23
		// 7. Esquina superior derecha
		0.05f, 2.44f,	-2.42f, 0.19f, 0.77f, -0.57f, -0.60f, 0.57f, // 24
		2.53f, -0.06f,	-2.58f, 0.13f, 0.62f, -0.57f, -0.60f, 0.57f, // 25
		2.52f, 2.45f,	0.06f, 0.06f, 0.77f, -0.57f, -0.60f, 0.57f, // 26
		// 8. Esquina superior izquierda posterior: 
		0.05f,	2.44f,	-2.42f, 0.22f, 0.78f, 0.57f, -0.59f, 0.57f, // 27
		-2.45f, -0.06f, -2.48f, 0.29f, 0.62f, 0.57f, -0.59f, 0.57f, // 28
		-2.45f, 2.42f,	0.08f, 0.35f, 0.77f, 0.57f, -0.59f, 0.57f, // 29
		// 9. Esquina superior izquierda frontal: 
		0.05f,	2.40f,	2.55f, 0.52f, 0.77f, 0.57f, -0.59f, -0.58f, // 30
		-2.65f, -0.14f, 2.49f, 0.45f, 0.62f, 0.57f, -0.59f, -0.58f, // 31
		-2.45f, 2.42f,	0.08f, 0.39f, 0.78f, 0.57f, -0.59f, -0.58f, // 32
		// 10. Esquina superior derecha frontal:
		0.05f,	2.40f,	2.55f, 0.56f, 0.77f, -0.57f, -0.58f, -0.58f, // 33
		2.59f,	-0.09f, 2.51f, 0.63f, 0.62f, -0.57f, -0.58f, -0.58f, // 34
		2.52f,	2.45f,	0.06f, 0.69f, 0.77f, -0.57f, -0.58f, -0.58f, // 35
		// 11. Esquina inferior izquierda posterior: 
		-2.52f, -2.58f, 0.00f, 0.81f, 0.62f, 0.55f, 0.58f, 0.60f, // 36
		0.08f,	-2.51f, -2.43f, 0.73f, 0.63f, 0.55f, 0.58f, 0.60f, // 37
		-2.45f, -0.06f, -2.48f, 0.77f, 0.78f, 0.55f, 0.58f, 0.60f, // 38
		// 12. Esquina inferior izquierda frontal: 
		-2.52f, -2.58f, 0.00f, 0.86f, 0.62f, 0.59f, 0.59f, -0.55f, // 39
		-2.65f, -0.14f, 2.49f, 0.91f, 0.78f, 0.59f, 0.59f, -0.55f, // 40
		-0.14f, -2.60f, 2.56f, 0.95f, 0.62f, 0.59f, 0.59f, -0.55f, // 41
		// 13. Esquina inferior derecha frontal:
		2.59f,	-0.09f, 2.51f, 0.11f, 0.59f, -0.55f, 0.59f, -0.59f, // 42
		-0.14f, -2.60f, 2.56f, 0.05f, 0.44f, -0.55f, 0.59f, -0.59f, // 43
		2.63f,	-2.55f, 0.01f, 0.16f, 0.44f, -0.55f, 0.59f, -0.59f, // 44
		// 14. Esquina inferior derecha posterior:
		2.53f,	-0.06f, -2.58f, 0.25f, 0.59f, -0.55f, 0.59f, 0.59f, // 45
		0.08f,	-2.51f, -2.43f, 0.31f, 0.44f, -0.55f, 0.59f, 0.59f, // 46
		2.63f,	-2.55f, 0.01f, 0.19f, 0.44f, -0.55f, 0.59f, 0.59f, // 47
	};

	unsigned int holocron_indices[] = {
		2, 1, 3, 2, 0, 1, // Cara 1
		4, 5, 6, 4, 7, 5, // Cara 2
		10, 8, 9, 10, 11, 8, // Cara 3
		14, 15, 12, 14, 13, 15, // Cara 4
		19, 16, 17, 19, 18, 16, // Cara 5
		20, 22, 23, 20, 23, 21, // Cara 6
		26, 25, 24, // Cara 7
		27, 28, 29, // Cara 8
		32, 31, 30, // Cara 9
		33, 34, 35, // Cara 10
		38, 37, 36, // Cara 11
		40, 39, 41, // Cara 12
		42, 43, 44, // Cara 13
		45, 47, 46, // Cara 14
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 60);
	meshListModel.push_back(holocron);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	logosStarWars = Texture("Textures/starwarslogos.png");
	logosStarWars.LoadTextureA();
	holocronTexture = Texture("Textures/holocronunwrap.png");
	holocronTexture.LoadTextureA();
	

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Rover_M = Model();
	Rover_M.LoadModel("Models/RoverModificado.obj");
	//Añadir modelo del avión
	Avion = Model();
	Avion.LoadModel("Models/avion.obj");
	//añadimos las rutas del skyboz

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

/**************************************				LUCES									*******************************************/
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.03f, 0.03f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	// Indices 0 y 1: linterna de la camara y luz fija.
	// 2: faro frontal del rover (+X local).

	spotLights[2] = SpotLight(1.0f, 0.95f, 0.8f,
		0.0f, 4.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.02f, 0.002f, 25.0f);
	spotLightCount++;

	// 3: faro inferior del avion (-Y local).
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 4.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 1.0f, 0.02f, 0.002f, 30.0f);
	spotLightCount++;

	// 4: faro frontal del avion (+Z local, hacia la helice).
	spotLights[4] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 4.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.02f, 0.002f, 25.0f);
	spotLightCount++;

	glm::vec3 posicionRover(1.0f, -1.0f, 6.0f);
	glm::vec3 posicionAvion(-25.5f, 2.5f, 0.0f);
	const glm::vec3 frenteRover(1.0f, 0.0f, 0.0f);
	const glm::vec3 frenteAvion(0.0f, 0.0f, 1.0f);
	const GLfloat velocidadRover = 4.0f; // unidades por segundo
	const GLfloat velocidadAvion = 6.0f;
	const GLfloat velocidadVertical = 3.0f;

	// Evitar que el tiempo de carga se convierta en movimiento del primer frame.
	lastTime = static_cast<GLfloat>(glfwGetTime());

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		const GLfloat segundosFrame = now - lastTime;
		deltaTime = segundosFrame;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Movimiento continuo mientras se mantengan presionadas las teclas.
		const bool* teclas = mainWindow.getsKeys();
		const GLfloat avanceRover = (teclas[GLFW_KEY_G] ? 1.0f : 0.0f)
			- (teclas[GLFW_KEY_F] ? 1.0f : 0.0f);
		const GLfloat avanceAvion = (teclas[GLFW_KEY_H] ? 1.0f : 0.0f)
			- (teclas[GLFW_KEY_J] ? 1.0f : 0.0f);
		const GLfloat alturaAvion = (teclas[GLFW_KEY_I] ? 1.0f : 0.0f)
			- (teclas[GLFW_KEY_K] ? 1.0f : 0.0f);

		posicionRover += frenteRover * avanceRover * velocidadRover * segundosFrame;
		posicionAvion += frenteAvion * avanceAvion * velocidadAvion * segundosFrame;
		posicionAvion.y += alturaAvion * velocidadVertical * segundosFrame;
		// El punto mas bajo del avion es -0.55 local, escalado por 2.
		if (posicionAvion.y < 0.1f) posicionAvion.y = 0.1f;

		// Matrices padre compartidas por los modelos y sus faros.
		glm::mat4 matrizRover = glm::translate(glm::mat4(1.0f), posicionRover);
		matrizRover = glm::scale(matrizRover, glm::vec3(0.4f));
		glm::mat4 matrizAvion = glm::translate(glm::mat4(1.0f), posicionAvion);
		matrizAvion = glm::scale(matrizAvion, glm::vec3(2.0f));

		// Actualizar antes de enviar las luces y dibujar incluso el piso.
		ActualizarFaro(spotLights[2], matrizRover,
			glm::vec3(5.3f, 5.0f, 0.0f), frenteRover);
		ActualizarFaro(spotLights[3], matrizAvion,
			glm::vec3(0.0f, -0.3f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f));
		ActualizarFaro(spotLights[4], matrizAvion,
			glm::vec3(0.0f, 0.15f, 2.75f), frenteAvion);

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();
	
		//BLACKHAWK
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();


		//Rover: Sustituir por su propio Rover con jerarquía
		model = matrizRover;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_M.RenderModel();


		//DADO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//dadoTexture.UseTexture();
		logosStarWars.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//HOLOCRON

		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, 4.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logosStarWars.UseTexture();
		meshListModel[5]->RenderMeshModel();

		//Añadir modelo del avión
		//AVION
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = matrizAvion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
