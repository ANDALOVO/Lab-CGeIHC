/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <algorithm>
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
// Unidades de mundo y segundos. Compartidas por el piso y sus recorridos.
namespace Practica9 {
constexpr double pi = 3.14159265358979323846;
constexpr float escalaPiso = 30.0f; // Piso de -300 a 300 en X y Z.
constexpr float mitadPiso = 10.0f * escalaPiso;
constexpr float alturaPiso = -2.0f;
constexpr float escalaRover = 0.5f;
constexpr double radioRueda = 0.6205;
// Limites fijos: dejar 4 unidades de margen antes de cada orilla del piso.
constexpr double minimoRover = -296.0;
constexpr double maximoRover = 296.0;
constexpr double velocidadRover = 30.0;
constexpr double radioVuelo = 280.0;
constexpr double velocidadAvion = 15.0;
constexpr float alturaVuelo = 12.0f;

struct Animacion {
    double xRover = 0.0;
    double giroRuedas = 0.0;
    bool avanza = true;
    double faseAvion = 0.0;

    void actualizar(double segundos) {
        if (!(segundos > 0.0) || !std::isfinite(segundos)) return;
        const double anterior = xRover;
        // Avanzar hasta la orilla derecha y retroceder hasta la izquierda.
        if (avanza) {
            xRover += velocidadRover * segundos;
            if (xRover >= 296.0) {
                xRover = 296.0;
                avanza = false;
            }
        } else {
            xRover -= velocidadRover * segundos;
            if (xRover <= -296.0) {
                xRover = -296.0;
                avanza = true;
            }
        }
        // Rodadura: angulo = distancia / radio. Hacia +X gira sobre -Z.
        giroRuedas = std::fmod(giroRuedas - (xRover - anterior) / radioRueda, 2.0 * pi);
        faseAvion = std::fmod(faseAvion + velocidadAvion / radioVuelo * segundos, 2.0 * pi);
    }
    double avionX() const { return radioVuelo * std::cos(faseAvion); }
    double avionZ() const { return radioVuelo * std::sin(faseAvion); }

    double rumboAvion() const { return -faseAvion; }
};
}
// Fin de la logica de animacion Practica9.
const float toRadians = 3.14159265f / 180.0f;


// Pista plana sobre el piso original. Unidades de mundo, avance en segundos.
namespace PistaSuelo {
constexpr float pi = 3.14159265358979323846f;
float longitud = 0.0f; // Longitud medida del recorrido.
constexpr float altura = -1.97f; // Piso en -2; separacion minima evita parpadeo.
constexpr float ancho = 14.0f;
constexpr float velocidad = 24.0f;
constexpr float alturaNave = 2.0f;
double distancia = 0.0;
MeshModel malla;
Texture textura;
struct Muestra { glm::vec3 posicion, frente; };
// Una elipse ondulada produce cuatro curvas amplias y cambios suaves de direccion.
// Tabla de distancia para que la nave no acelere artificialmente en las curvas.
std::vector<Muestra> recorrido;
std::vector<float> distancias;
void prepararRecorrido() {
    recorrido.clear(); distancias.clear(); longitud=0;
    const int pasos=1024;
    for(int i=0;i<pasos;++i) {
        float a=2*pi*i/pasos;
        float r=1+0.16f*std::cos(4*a);
        float dr=-0.64f*std::sin(4*a);
        glm::vec3 p(125*r*std::cos(a),altura,-125+70*r*std::sin(a));
        glm::vec3 f(125*(dr*std::cos(a)-r*std::sin(a)),0,70*(dr*std::sin(a)+r*std::cos(a)));
        recorrido.push_back({p,glm::normalize(f)});
    }
    for(int i=0;i<pasos;++i) {
        distancias.push_back(longitud);
        longitud+=glm::length(recorrido[(i+1)%pasos].posicion-recorrido[i].posicion);
    }
    distancias.push_back(longitud);
}
Muestra muestra(double distanciaRecorrida) {
    if(recorrido.empty()) return {{0,altura,-125},{1,0,0}};
    float d=static_cast<float>(std::fmod(distanciaRecorrida,static_cast<double>(longitud)));
    if(d<0) d+=longitud;
    if(d>=longitud) d=0;
    size_t i=std::upper_bound(distancias.begin(),distancias.end(),d)-distancias.begin()-1;
    float t=(d-distancias[i])/(distancias[i+1]-distancias[i]);
    const auto& a=recorrido[i]; const auto& b=recorrido[(i+1)%recorrido.size()];
    return {glm::mix(a.posicion,b.posicion,t),glm::normalize(glm::mix(a.frente,b.frente,t))};
}
void crear() {
    prepararRecorrido();
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    const int pasos=480;
    for(int i=0;i<=pasos;++i) {
        auto p=muestra(longitud*static_cast<double>(i)/pasos);
        glm::vec3 lateral=glm::cross(p.frente,glm::vec3(0,1,0));
        for(int lado=0;lado<2;++lado) {
            glm::vec3 v=p.posicion+lateral*((lado?1.0f:-1.0f)*ancho/2);
            vertices.insert(vertices.end(), {v.x,v.y,v.z,static_cast<float>(lado),i*40.0f/pasos,0,-1,0});
        }
        if(i<pasos) {
            unsigned int a=i*2;
            indices.insert(indices.end(), {a,a+1,a+2,a+1,a+3,a+2});
        }
    }
    malla.CreateMeshModel(vertices.data(),indices.data(),static_cast<unsigned int>(vertices.size()),static_cast<unsigned int>(indices.size()));
    // Textura repetible de asfalto, bordes blancos y linea central discontinua.
    std::vector<unsigned char> pixels(64*64*4);
    for(int y=0;y<64;++y) for(int x=0;x<64;++x) {
        int k=(y*64+x)*4;
        unsigned char gris=static_cast<unsigned char>(53+(x*17+y*31)%17);
        bool borde=(x>=3&&x<=4)||(x>=59&&x<=60);
        bool centro=(x>=31&&x<=32)&&y<36;
        pixels[k]=borde?220:centro?235:gris;
        pixels[k+1]=borde?220:centro?190:gris;
        pixels[k+2]=borde?220:centro?65:gris;
        pixels[k+3]=255;
    }
    textura.LoadRGBA(pixels.data(),64,64);
}
}
// Fin de pista a nivel de piso.

//variables para animación
Practica9::Animacion animacion;

float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;


Window mainWindow(1366, 768);
std::vector<MeshModel*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;



Model Rover_M;
Model Ruedas_M[6];
Model Avion_M;
Model Nave_M;
Model Satelite_M;
namespace SateliteVuelo {
constexpr double periodo = 55.0; // Segundos por vuelta.
constexpr float altura = 65.0f;
constexpr float escala = 12.0f; // STL original: ancho aproximado de 1.76 unidades.
// Centro del STL medido para que rote y vuele desde su propio centro.
const glm::vec3 centroModelo(0.0437836f,-0.9203133f,0.1717712f);
double fase = 0.0;
}
const char* nombresRuedas[] = {
    "rueda_delantera_izquierda", "rueda_delantera_derecha",
    "rueda_media_izquierda", "rueda_media_derecha",
    "rueda_trasera_izquierda", "rueda_trasera_derecha"
};
const glm::vec3 pivotesRuedas[] = {
    {5.709923f, 1.704993f, -3.070003f},
    {5.712381f, 1.704992f,  3.070000f},
    {-0.947183f, 1.412098f, -4.158950f},
    {-0.944719f, 1.412099f,  4.158949f},
    {-5.113412f, 1.385755f, -4.158949f},
    {-5.130509f, 1.412099f,  4.158949f}
};



Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
double lastTime = 0.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//cálculo del promedio de las normales para sombreado de Phong
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
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	MeshModel* obj5 = new MeshModel();
	obj5->CreateMeshModel(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	MeshModel* obj6 = new MeshModel();
	obj6->CreateMeshModel(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	MeshModel* obj7 = new MeshModel();
	obj7->CreateMeshModel(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	PistaSuelo::crear();
	Nave_M.LoadModel("Models/nave.obj");
	Satelite_M.LoadModel("Models/satelite.stl");

	camera = Camera(glm::vec3(40.0f, 30.0f, 52.0f), glm::vec3(0.0f, 1.0f, 0.0f), -127.5f, -24.0f, 15.0f, 0.5f);

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
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	Rover_M = Model();
	Rover_M.LoadModel("Models/RoverJerarquizado/cuerpo.obj");
    for (int i = 0; i < 6; ++i)
        Ruedas_M[i].LoadModel(std::string("Models/RoverJerarquizado/") + nombresRuedas[i] + ".obj");
    Avion_M.LoadModel("Models/avion.obj");
	

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


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	



	glm::vec3 lowerLight(0.0f,0.0f,0.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	lastTime = glfwGetTime(); // No contar el tiempo de carga de modelos.
	while (!mainWindow.getShouldClose())
	{
        const double now = glfwGetTime();
        const double segundosFrame = now - lastTime;
        lastTime = now;
        deltaTime = static_cast<GLfloat>(segundosFrame);
        animacion.actualizar(segundosFrame);
        SateliteVuelo::fase = std::fmod(SateliteVuelo::fase + 2.0*Practica9::pi*segundosFrame/SateliteVuelo::periodo,2.0*Practica9::pi);
        PistaSuelo::distancia = std::fmod(PistaSuelo::distancia + PistaSuelo::velocidad*segundosFrame, static_cast<double>(PistaSuelo::longitud));
        angulovaria += 30.0f * deltaTime;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

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
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		//Reinicializando variables cada ciclo de reloj
		model= glm::mat4(1.0);
		modelaux= glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, Practica9::alturaPiso, 0.0f));
		model = glm::scale(model, glm::vec3(Practica9::escalaPiso, 1.0f, Practica9::escalaPiso));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMeshModel();



        model=glm::mat4(1);
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
        PistaSuelo::textura.UseTexture();
        PistaSuelo::malla.RenderMeshModel();
        const auto nave=PistaSuelo::muestra(PistaSuelo::distancia);
        model=glm::translate(glm::mat4(1),nave.posicion+glm::vec3(0,PistaSuelo::alturaNave,0));
        // Frente del OBJ hacia -Z: corregir 180 grados respecto a la tangente.
        model=glm::rotate(model,std::atan2(nave.frente.x,nave.frente.z)+PistaSuelo::pi,glm::vec3(0,1,0));
        model=glm::scale(model,glm::vec3(0.035f));
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity,uniformShininess);
        Nave_M.RenderModel();




        // Padre: traslacion del rover. Se conserva el frente +X al retroceder.
        // Apoyar el punto inferior del modelo (Y local 0.1449) sobre el piso.
        const glm::mat4 matrizRover = glm::scale(
            glm::translate(glm::mat4(1.0f), glm::vec3(
                static_cast<float>(animacion.xRover),
                -2.07245f, 0.0f)),
            glm::vec3(Practica9::escalaRover));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(matrizRover));
        Rover_M.RenderModel();
        for (int i = 0; i < 6; ++i) {
            // Hija: M_rover * T_pivote * R_giro. Vertices centrados en el eje.
            model = glm::translate(matrizRover, pivotesRuedas[i]);
            model = glm::rotate(model, static_cast<float>(animacion.giroRuedas), glm::vec3(0, 0, 1));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            Ruedas_M[i].RenderModel();
        }

        // Vuelo circular continuo. La nariz +Z sigue la tangente de la curva.
        model = glm::translate(glm::mat4(1.0f), glm::vec3(
            static_cast<float>(animacion.avionX()), Practica9::alturaVuelo,
            static_cast<float>(animacion.avionZ())));
        model = glm::rotate(model, static_cast<float>(animacion.rumboAvion()), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(1.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Avion_M.RenderModel();

        // Satelite STL: orbita eliptica con variacion suave de altura.
        const float faseSat=static_cast<float>(SateliteVuelo::fase);
        glm::vec3 posicionSat(170*std::cos(faseSat),SateliteVuelo::altura+8*std::sin(2*faseSat),-125+100*std::sin(faseSat));
        glm::vec3 tangenteSat(-170*std::sin(faseSat),16*std::cos(2*faseSat),100*std::cos(faseSat));
        float rumboSat=std::atan2(tangenteSat.x,tangenteSat.z);
        float inclinacionSat=-std::atan2(tangenteSat.y,glm::length(glm::vec2(tangenteSat.x,tangenteSat.z)));
        model=glm::translate(glm::mat4(1),posicionSat);
        model=glm::rotate(model,rumboSat,glm::vec3(0,1,0));
        model=glm::rotate(model,inclinacionSat,glm::vec3(1,0,0));
        model=glm::scale(model,glm::vec3(SateliteVuelo::escala));
        model=glm::translate(model,-SateliteVuelo::centroModelo);
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
        glUniform3f(uniformColor,0.8f,0.85f,0.95f);
        Material_brillante.UseMaterial(uniformSpecularIntensity,uniformShininess);
        Satelite_M.RenderModel();
        glUniform3f(uniformColor,1,1,1);


		//Agave ¿qué sucede si lo renderizan antes del Rover?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMeshModel();
		
		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMeshModel();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMeshModel();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMeshModel();

		for (int i = 1; i<4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMeshModel();

		 }

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMeshModel();
		}
 

		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		toffsetnumerocambiau += 0.25; 
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMeshModel();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
		//if
		//Numero1Texture.UseTexture();
		//Numero2Texture.UseTexture();
		
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMeshModel();

		glDisable(GL_BLEND);
		
		



		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
