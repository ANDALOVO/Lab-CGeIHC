/*
Práctica 6: Texturizado
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

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz

std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture holocronTexture;
Texture logosStarWars;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Holocron_M;
Model Avion;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";





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

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
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
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
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
		// x            y            z          S       T        NX    NY    NZ
	// cara 0: Rebel_Alliance  (4 vertices, 2 triangulos)
		   -2.453980,   -0.063388,   -2.477451,   0.1517, 0.8355,   0.0f, 0.0f, 0.0f,
		   -1.185055,   -2.530499,   -1.198261,   0.1519, 0.9552,   0.0f, 0.0f, 0.0f,
		   -2.517274,   -2.579095,    0.002115,   0.0971, 0.9549,   0.0f, 0.0f, 0.0f,
		   -2.453980,   -0.063388,   -2.477451,   0.1517, 0.8355,   0.0f, 0.0f, 0.0f,
			0.081432,   -2.513204,   -2.426005,   0.2058, 0.9549,   0.0f, 0.0f, 0.0f,
		   -1.185055,   -2.530499,   -1.198261,   0.1519, 0.9552,   0.0f, 0.0f, 0.0f,
	// cara 1: New_Republic  (4 vertices, 2 triangulos)
			-2.453980,   -0.063388,   -2.477451,   0.3192, 0.9647,   0.0f, 0.0f, 0.0f,
			-2.646786,   -0.139047,    2.493757,   0.3191, 0.8260,   0.0f, 0.0f, 0.0f,
			-2.451372,    2.421759,    0.077538,   0.3734, 0.8945,   0.0f, 0.0f, 0.0f,
			-2.453980,   -0.063388,   -2.477451,   0.3192, 0.9647,   0.0f, 0.0f, 0.0f,
			-2.517274,   -2.579095,    0.002115,   0.2659, 0.8945,   0.0f, 0.0f, 0.0f,
			-2.646786,   -0.139047,    2.493757,   0.3191, 0.8260,   0.0f, 0.0f, 0.0f,
	// cara 2: Galactic_Empire  (3 vertices, 1 triangulo)
			2.521206,    2.448241,    0.063388,   0.5366, 0.9528,   0.0f, 0.0f, 0.0f,
			2.534944,   -0.061146,   -2.580280,   0.4257, 0.9528,   0.0f, 0.0f, 0.0f,
			0.049649,    2.440128,   -2.417217,   0.4820, 0.8379,   0.0f, 0.0f, 0.0f,
	// cara 3: Lord_Revan  (3 vertices, 1 triangulo)
			0.049649,    2.440128,   -2.417217,   0.6339, 0.9551,   0.0f, 0.0f, 0.0f,
			-2.453980,   -0.063388,   -2.477451,   0.5918, 0.8356,   0.0f, 0.0f, 0.0f,
			-2.451372,    2.421759,    0.077538,   0.6757, 0.8356,   0.0f, 0.0f, 0.0f,
	// cara 4: Mandalorian_Crest  (4 vertices, 2 triangulos)
			-0.140696,   -2.601934,    2.558561,   0.7701, 0.8289,   0.0f, 0.0f, 0.0f,
			0.051152,    2.399781,    2.545043,   0.7752, 0.9618,   0.0f, 0.0f, 0.0f,
			-2.646786,   -0.139047,    2.493757,   0.7176, 0.8951,   0.0f, 0.0f, 0.0f,
			-0.140696,   -2.601934,    2.558561,   0.7701, 0.8289,   0.0f, 0.0f, 0.0f,
			2.593003,   -0.085222,    2.505755,   0.8285, 0.8951,   0.0f, 0.0f, 0.0f,
			0.051152,    2.399781,    2.545043,   0.7752, 0.9618,   0.0f, 0.0f, 0.0f,
	// cara 5: Black_Sun  (3 vertices, 1 triangulo)
			-2.451372,    2.421759,    0.077538,   0.9060, 0.8390,   0.0f, 0.0f, 0.0f,
			-2.646786,   -0.139047,    2.493757,   0.9509, 0.9517,   0.0f, 0.0f, 0.0f,
			0.051152,    2.399781,    2.545043,   0.8616, 0.9517,   0.0f, 0.0f, 0.0f,
	// cara 6: Revanchist_SITH  (3 vertices, 1 triangulo)
			0.051152,    2.399781,    2.545043,   0.0706, 0.6366,   0.0f, 0.0f, 0.0f,
			2.593003,   -0.085222,    2.505755,   0.1858, 0.6366,   0.0f, 0.0f, 0.0f,
			2.521206,    2.448241,    0.063388,   0.1276, 0.7587,   0.0f, 0.0f, 0.0f,
	// cara 7: Old_Galactic_Republic  (4 vertices, 2 triangulos)
			-2.646786,   -0.139047,    2.493757,   0.2337, 0.7591,   0.0f, 0.0f, 0.0f,
			-1.328995,   -2.578386,    1.297838,   0.3166, 0.6984,   0.0f, 0.0f, 0.0f,
			-0.140696,   -2.601934,    2.558561,   0.3446, 0.7591,   0.0f, 0.0f, 0.0f,
			-2.646786,   -0.139047,    2.493757,   0.2337, 0.7591,   0.0f, 0.0f, 0.0f,
			-2.517274,   -2.579095,    0.002115,   0.2891, 0.6362,   0.0f, 0.0f, 0.0f,
			-1.328995,   -2.578386,    1.297838,   0.3166, 0.6984,   0.0f, 0.0f, 0.0f,
	// cara 8: Galactic_Republic  (4 vertices, 2 triangulos)
			2.593003,   -0.085222,    2.505755,   0.4625, 0.7562,   0.0f, 0.0f, 0.0f,
			1.385028,   -2.549476,    1.308677,   0.4593, 0.6424,   0.0f, 0.0f, 0.0f,
			2.632201,   -2.553892,    0.007226,   0.5122, 0.6391,   0.0f, 0.0f, 0.0f,
			2.593003,   -0.085222,    2.505755,   0.4625, 0.7562,   0.0f, 0.0f, 0.0f,
			-0.140696,   -2.601934,    2.558561,   0.4013, 0.6391,   0.0f, 0.0f, 0.0f,
			1.385028,   -2.549476,    1.308677,   0.4593, 0.6424,   0.0f, 0.0f, 0.0f,
	// cara 9: Rebels_Phoenix  (4 vertices, 2 triangulos)
			2.534944,   -0.061146,   -2.580280,   0.5711, 0.6378,   0.0f, 0.0f, 0.0f,
			1.344182,   -2.542571,   -1.294387,   0.6526, 0.6994,   0.0f, 0.0f, 0.0f,
			0.081432,   -2.513204,   -2.426005,   0.6255, 0.7575,   0.0f, 0.0f, 0.0f,
			2.534944,   -0.061146,   -2.580280,   0.5711, 0.6378,   0.0f, 0.0f, 0.0f,
			2.632201,   -2.553892,    0.007226,   0.6831, 0.6378,   0.0f, 0.0f, 0.0f,
			1.344182,   -2.542571,   -1.294387,   0.6526, 0.6994,   0.0f, 0.0f, 0.0f,
	// cara 10: Republic_Credits  (4 vertices, 2 triangulos)
		0.081432,   -2.513204,   -2.426005,   0.7726, 0.6265,   0.0f, 0.0f, 0.0f,
		0.049649,    2.440128,   -2.417217,   0.7730, 0.7688,   0.0f, 0.0f, 0.0f,
		2.534944,   -0.061146,   -2.580280,   0.7434, 0.6969,   0.0f, 0.0f, 0.0f,
		0.081432,   -2.513204,   -2.426005,   0.7726, 0.6265,   0.0f, 0.0f, 0.0f,
		-2.453980,   -0.063388,   -2.477451,   0.8027, 0.6969,   0.0f, 0.0f, 0.0f,
		0.049649,    2.440128,   -2.417217,   0.7730, 0.7688,   0.0f, 0.0f, 0.0f,
	// cara 11: Boba_Fett_Crest  (8 vertices, 6 triangulos)
			1.344182,   -2.542571,   -1.294387,   0.9244, 0.6598,   0.0f, 0.0f, 0.0f,
			1.385028,   -2.549476,    1.308677,   0.9251, 0.7322,   0.0f, 0.0f, 0.0f,
			-1.328995,   -2.578386,    1.297838,   0.8809, 0.7320,   0.0f, 0.0f, 0.0f,
			-1.328995,   -2.578386,    1.297838,   0.8809, 0.7320,   0.0f, 0.0f, 0.0f,
			-2.517274,   -2.579095,    0.002115,   0.8615, 0.6959,   0.0f, 0.0f, 0.0f,
			-1.185055,   -2.530499,   -1.198261,   0.8832, 0.6625,   0.0f, 0.0f, 0.0f,
			 -1.185055,   -2.530499,   -1.198261,   0.8832, 0.6625,   0.0f, 0.0f, 0.0f,
			0.081432,   -2.513204,   -2.426005,   0.9038, 0.6283,   0.0f, 0.0f, 0.0f,
			1.344182,   -2.542571,   -1.294387,   0.9244, 0.6598,   0.0f, 0.0f, 0.0f,
			1.344182,   -2.542571,   -1.294387,   0.9244, 0.6598,   0.0f, 0.0f, 0.0f,
			2.632201,   -2.553892,    0.007226,   0.9454, 0.6959,   0.0f, 0.0f, 0.0f,
			1.385028,   -2.549476,    1.308677,   0.9251, 0.7322,   0.0f, 0.0f, 0.0f,
			1.385028,   -2.549476,    1.308677,   0.9251, 0.7322,   0.0f, 0.0f, 0.0f,
			 -0.140696,   -2.601934,    2.558561,   0.9003, 0.7670,   0.0f, 0.0f, 0.0f,
			 -1.328995,   -2.578386,    1.297838,   0.8809, 0.7320,   0.0f, 0.0f, 0.0f,
			-1.328995,   -2.578386,    1.297838,   0.8809, 0.7320,   0.0f, 0.0f, 0.0f,
			-1.185055,   -2.530499,   -1.198261,   0.8832, 0.6625,   0.0f, 0.0f, 0.0f,
			1.344182,   -2.542571,   -1.294387,   0.9244, 0.6598,   0.0f, 0.0f, 0.0f,
	// cara 12: SITH_Empire  (4 vertices, 2 triangulos)
			2.632201,   -2.553892,    0.007226,   0.1022, 0.4424,   0.0f, 0.0f, 0.0f,
			2.521206,    2.448241,    0.063388,   0.1016, 0.5808,   0.0f, 0.0f, 0.0f,
			2.593003,   -0.085222,    2.505755,   0.0548, 0.5110,   0.0f, 0.0f, 0.0f,
			2.632201,   -2.553892,    0.007226,   0.1022, 0.4424,   0.0f, 0.0f, 0.0f,
			2.534944,   -0.061146,   -2.580280,   0.1517, 0.5110,   0.0f, 0.0f, 0.0f,
			2.521206,    2.448241,    0.063388,   0.1016, 0.5808,   0.0f, 0.0f, 0.0f,
	// cara 13: Separatists  (4 vertices, 2 triangulos)
			0.049649,    2.440128,   -2.417217,   0.2498, 0.4414,   0.0f, 0.0f, 0.0f,
			0.051152,    2.399781,    2.545043,   0.2500, 0.5818,   0.0f, 0.0f, 0.0f,
			2.521206,    2.448241,    0.063388,   0.1937, 0.5118,   0.0f, 0.0f, 0.0f,
			0.049649,    2.440128,   -2.417217,   0.2498, 0.4414,   0.0f, 0.0f, 0.0f,
			-2.451372,    2.421759,    0.077538,   0.3068, 0.5118,   0.0f, 0.0f, 0.0f,
			0.051152,    2.399781,    2.545043,   0.2500, 0.5818,   0.0f, 0.0f, 0.0f,
	};

	unsigned int holocron_indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35,
		36, 37, 38,
		39, 40, 41,
		42, 43, 44,
		45, 46, 47,
		48, 49, 50,
		51, 52, 53,
		54, 55, 56,
		57, 58, 59,
		60, 61, 62,
		63, 64, 65,
		66, 67, 68,
		69, 70, 71,
		72, 73, 74,
		75, 76, 77,
		78, 79, 80,
		81, 82, 83,
	};

	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 672, 84);
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

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	dadoTexture = Texture("Textures/dado-de-numeros.png");
	dadoTexture.LoadTextureA();
	logosStarWars = Texture("Textures/starwarslogos.png");
	logosStarWars.LoadTextureA();

	holocronTexture = Texture("Textures/holocronunwrap.png");
	holocronTexture.LoadTextureA();

	
	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holocrontexturizado.obj");

	Dado_M = Model();
	Dado_M.LoadModel("Models/logosStarWars.obj");

	Avion = Model();
	Avion.LoadModel("Models/avion.obj");

	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshListModel[2]->RenderMeshModel();




		//Dado de Opengl
		//Ejercicio 1: Texturizar su dado con la imagen ya optimizada por ustedes con logos de star wars
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		logosStarWars.UseTexture();
		meshListModel[4]->RenderMeshModel();
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen ya optimizada por ustedes
		
		
		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();
		

		

/*		Reporte de práctica :
		
		Ejercicio 1: Crear o modificar el holocron y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo del holocron texturizardo en el programa de modelado
		Ejercicio 3: Importar un modelo de avión con con la textura de la cara del personaje de la imagen del previo:
		Vidrio fonrtal: OJOS
		Frente del avión: Nariz y Sonrisa
		Alas: Logos del universo del personaje
		
		*/


		//Holocrones
		//color = glm::vec3(0.0f, 1.0f, 0.0f);//color que multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, 4.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		logosStarWars.UseTexture();
		meshListModel[5]->RenderMeshModel();
		

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.5f, 2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron_M.RenderModel();



		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.5f, 2.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/