//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>

//glm
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

//clases para dar orden y limpieza al código
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0f; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;

//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

float angulo = 0.0f;

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}
//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
		// X       Y       Z        R       G       B

		// --- LETRA A (Color Rojo) ---
		-0.8f,   0.0f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.9f,   0.0f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.7f,   0.4f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.8f,   0.0f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.65f,  0.3f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.7f,   0.4f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.6f,   0.0f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.5f,   0.0f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.7f,   0.4f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.6f,   0.0f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.75f,  0.3f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.7f,   0.4f,   0.0f,    1.0f,   0.0f,   0.0f,
		-0.8f,   0.08f,  0.0f,    1.0f,   0.0f,   0.0f,
		-0.8f,   0.12f,  0.0f,    1.0f,   0.0f,   0.0f,
		-0.6f,   0.075f, 0.0f,    1.0f,   0.0f,   0.0f,
		-0.6f,   0.075f, 0.0f,    1.0f,   0.0f,   0.0f,
		-0.6f,   0.12f,  0.0f,    1.0f,   0.0f,   0.0f,
		-0.8f,   0.12f,  0.0f,    1.0f,   0.0f,   0.0f,

		// --- LETRA L (Color Verde) ---
		-0.4f,   0.0f,   0.0f,    0.0f,   1.0f,   0.0f,
		-0.1f,   0.0f,   0.0f,    0.0f,   1.0f,   0.0f,
		-0.4f,   0.1f,   0.0f,    0.0f,   1.0f,   0.0f,

		-0.1f,   0.0f,   0.0f,    0.0f,   1.0f,   0.0f,
		-0.1f,   0.1f,   0.0f,    0.0f,   1.0f,   0.0f,
		-0.4f,   0.1f,   0.0f,    0.0f,   1.0f,   0.0f,

		-0.4f,   0.0f,   0.0f,    0.0f,   1.0f,   0.0f,
		-0.3f,   0.0f,   0.0f,    0.0f,   1.0f,   0.0f,
		-0.4f,   0.4f,   0.0f,    0.0f,   1.0f,   0.0f,

		-0.3f,   0.0f,   0.0f,    0.0f,   1.0f,   0.0f,
		-0.3f,   0.4f,   0.0f,    0.0f,   1.0f,   0.0f,
		-0.4f,   0.4f,   0.0f,    0.0f,   1.0f,   0.0f,

		// --- LETRA V (Color Amarillo) ---
		 0.25f,  0.0f,   0.0f,    1.0f,   1.0f,   0.0f,
		 0.05f,  0.4f,   0.0f,    1.0f,   1.0f,   0.0f,
		 0.15f,  0.4f,   0.0f,    1.0f,   1.0f,   0.0f,

		 0.25f,  0.0f,   0.0f,    1.0f,   1.0f,   0.0f,
		 0.15f,  0.4f,   0.0f,    1.0f,   1.0f,   0.0f,
		 0.25f,  0.2f,   0.0f,    1.0f,   1.0f,   0.0f,

		 0.25f,  0.0f,   0.0f,    1.0f,   1.0f,   0.0f,
		 0.35f,  0.4f,   0.0f,    1.0f,   1.0f,   0.0f,
		 0.45f,  0.4f,   0.0f,    1.0f,   1.0f,   0.0f,

		 0.25f,  0.0f,   0.0f,    1.0f,   1.0f,   0.0f,
		 0.35f,  0.4f,   0.0f,    1.0f,   1.0f,   0.0f,
		 0.25f,  0.2f,   0.0f,    1.0f,   1.0f,   0.0f
	};

	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 396);
	meshColorList.push_back(letras);

	// Triángulo Rojo Extra
	GLfloat vertices_triangulorojo[] = {
		-1.0f, -1.0f, 0.5f,   1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.5f,   1.0f, 0.0f, 0.0f,
		 0.0f,  1.0f, 0.5f,   1.0f, 0.0f, 0.0f,
	};
	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	// Cuadrado Verde Extra
	GLfloat vertices_cuadradoverde[] = {
		-0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
	};
	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader); // Índice 0 (Clamp)
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShaderColor); // Índice 1 (Letras)
	shaderList.push_back(*shader2);

	Shader* shaderVerde = new Shader();
	shaderVerde->CreateFromFiles(vShader, "shaders/verde.frag"); // Índice 2
	shaderList.push_back(*shaderVerde);

	Shader* shaderAzul = new Shader();
	shaderAzul->CreateFromFiles(vShader, "shaders/azul.frag"); // Índice 3
	shaderList.push_back(*shaderAzul);

	Shader* shaderCafe = new Shader();
	shaderCafe->CreateFromFiles(vShader, "shaders/cafe.frag"); // Índice 4
	shaderList.push_back(*shaderCafe);

	Shader* shaderMagenta = new Shader();
	shaderMagenta->CreateFromFiles(vShader, "shaders/magenta.frag"); // Índice 5
	shaderList.push_back(*shaderMagenta);

	
	Shader* shaderNegro = new Shader();
	shaderNegro->CreateFromFiles(vShader, "shaders/negro.frag");// Índice 6: Shader NEGRO
	shaderList.push_back(*shaderNegro);

	Shader* shaderAmarillo = new Shader();
	shaderAmarillo->CreateFromFiles(vShader, "shaders/amarillo.frag");// Índice 7: Shader AMARILLO
	shaderList.push_back(*shaderAmarillo);

	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShader, "shaders/rojo.frag");// Índice 8: Shader rojo
	shaderList.push_back(*shaderRojo);

}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	// Puedes dejar estas llamadas, pero no se dibujarán hasta que tú lo indiques
	CreaPiramide();
	CrearCubo();
	CrearPiramideCuadrangular();
	CrearLetrasyFiguras();
	CreateShaders();

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	// Matriz ortográfica para ver las letras planas (2D)
	// glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
// HABILITAR PROFUNDIDAD PARA EL 3D
	glEnable(GL_DEPTH_TEST);

	// CAMBIAR A PERSPECTIVA PARA VER LA PROFUNDIDAD
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)mainWindow.getBufferWidth() / (float)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	glm::mat4 model(1.0);

	while (!mainWindow.getShouldClose())
	{
		glfwPollEvents();

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		angulo += 0.0001f; // Para rotar las figuras

		// ========================================================
		// 1. DIBUJAR LETRAS (ALV) - Índice 1 de Shaders
		// ========================================================
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 1.5f, -6.0f)); // Arriba a la izquierda
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		// ========================================================
		// ESTRUCTURA 1: TORRE IZQUIERDA (Pilares y Pirámides)
		// ========================================================

		// 1. Poste izquierdo (Cubo Café estirado)
		shaderList[4].useShader(); // 4 = Café
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.78f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.0f, 0.05f)); // Estirado en Y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); // Cubo

		// 2. Poste derecho (Cubo Café estirado)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.42f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.0f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh(); // Cubo

		// 3. Pirámide Arriba (Magenta)
		shaderList[7].useShader(); // 5 = Magenta
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, 0.3f, -5.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // Apuntando abajo
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh(); // Pirámide triangular

		// 4. Pirámide Medio (Azul)
		shaderList[8].useShader(); // 3 = Azul
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, 0.0f, -5.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// 5. Pirámide Abajo (Verde)
		shaderList[2].useShader(); // 2 = Verde
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, -0.3f, -5.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		// ========================================================
		// ESTRUCTURA 2: CUADRO CENTRAL (Fijo y en capas)
		// ========================================================
		
		// 5. Cubo cafe del centro
		shaderList[4].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		// Lo acercamos a la cámara (Z = -4.7f) para que resalte por delante del magenta
		model = glm::translate(model, glm::vec3(-0.0f, -0.28f, -4.7f));
		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.2f, 0.2f, 1.0f));
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// 2. Rombo Medio (Cubo Azul) - Capa media
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		// Lo acercamos a la cámara (Z = -4.7f) para que resalte por delante del magenta
		model = glm::translate(model, glm::vec3(0.0f, -0.28f, -4.7f));
		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		// 1. Base Izquierda (Pirámide Verde)
		shaderList[2].useShader(); // Verde
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.15f, -0.45f, -5.0f));
		model = glm::rotate(model, 225.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // Apuntando abajo
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh(); // Pirámide triangular

		// 2. Base Derecha (Pirámide magenta)
		shaderList[5].useShader(); // magenta
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.15f, -0.45f, -5.0f));
		model = glm::rotate(model, 135.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // Apuntando abajo
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// 3. Punta Superior (Pirámide rojo)
		shaderList[8].useShader(); // rojo
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.15f, -0.15f, -5.0f));
		model = glm::rotate(model, 315.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // Apuntando abajo
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Pirámide amarilla
		shaderList[7].useShader(); // amarillo
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.15f, -0.15f, -5.0f));
		model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // Apuntando abajo
		model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();




		// ========================================================
		// ESTRUCTURA 3: PIRÁMIDE DERECHA (Trifuerza 3D)
		// ========================================================

		// 1. Base Izquierda (Pirámide Verde)
		shaderList[2].useShader(); // Verde
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.55f, -0.35f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh(); // Pirámide triangular

		// 2. Base Derecha (Pirámide rojo)
		shaderList[8].useShader(); // rojo
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.85f, -0.35f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// 3. Punta Superior (Pirámide Azul)
		shaderList[5].useShader(); // Azul
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.7f,-0.05f, -5.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// 4. Centro Invertido (Pirámide amarillo)
		shaderList[7].useShader(); // amarillo
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.7f, -0.35f, -5.0f));
		model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f)); // Apuntando abajo
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		// ========================================================
		// CUBO NEGRO ESTIRADO HORIZONTALMENTE
		// ========================================================
		shaderList[6].useShader(); // Activamos el color negro
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		model = glm::mat4(1.0);
		// Lo colocamos un poco abajo en la pantalla
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -5.0f));

		// ¡El truco de la escala! X = 2.0 (muy ancho), Y = 0.1 (muy bajito)
		model = glm::scale(model, glm::vec3(2.0f, 0.01f, 0.3f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[1]->RenderMesh(); // Mandamos llamar al cubo (Índice 1)
/*****************************************/
		
		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}