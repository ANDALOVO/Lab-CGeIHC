#pragma once
// Adaptado de Codigo/proyFinalMain.cpp del equipo.
namespace Equipo {
const float toRadians = 3.14159265f / 180.0f;


std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;



Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoPlaneta2;

// Añadir Texturas a usar

// Variables para los planetas 
float rotaPlanetas = 0.0f;
float rotaPlanetasOffset = 0.005f;

float rotaAnillo = 0.0f;
float rotaAnilloOffset = 0.0025f;

float rotaLuna = 0.0f;
float rotaLunaOffset = 0.005f;

// Variables para el satelite
float movSatelite = 0.0f;
float movSateliteOffset = 0.002f;

int estadoSatelite = 0;
float rotacionSatelite = 0.0f;

float rotaAntena = 0.0f;
float rotaAntenaOffset = 0.025f;

float rotaPanelIzq = 0.0f;
float rotaPanelDer = 0.0f;

float rotaPanelIzqOffset = 0.01f;
float rotaPanelDerOffset = 0.01f;

// Variables para rocky
float rotBrazoRocky = 0.0f;
float rotBrazoRockyOffset = 0.1f;
bool saludo = true;

// ----- Lista de Modelos a importar -----

// Planeta 2
Model rocky;
Model brazoRocky;
Model rocas1;

//Añadir modelo del avión. 
Model Avion_M; 

// Modelos del espacio
Model sol;
Model luna;
Model anilloAst;
Model planeta1;
Model planeta2;
Model cuerpoSat;
Model antenaSat;
Model panelDerSat;
Model panelIzqSat;
Model asteroides;


// Skybox de nuestros escenarios
Skybox skybox1;		// Skybox del espacio
Skybox skybox2;		// Skybox del planeta 1
Skybox skybox3;		// Skybox del planeta 2

int escenarioActual = 1;	// Escenario actual a mostrar (1, 2 o 3)

// Variables para animación


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






void iniciarBase() {
	CreateObjects();
	CreateShaders();


	brickTexture = Texture("Textures/Equipo/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/Equipo/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/Equipo/plain.png");
	plainTexture.LoadTextureA();
	pisoPlaneta2 = Texture("Textures/Equipo/pisoPlaneta2.jpg");
	pisoPlaneta2.LoadTextureA();

	// ----- Carga de modelos -----
	// Rocky 
	rocky = Model();
	rocky.LoadModel("Models/Equipo/rockyPersonaje.obj");

	brazoRocky = Model();
	brazoRocky.LoadModel("Models/Equipo/rockyBrazo.obj");

	// Piso Planeta 2
	
	rocas1 = Model();
	rocas1.LoadModel("Models/Equipo/rocas1.obj");

	// Modelos del espacio
	sol = Model();
	sol.LoadModel("Models/Equipo/sol.obj");

	luna = Model();
	luna.LoadModel("Models/Equipo/luna.obj");

	anilloAst = Model();
	anilloAst.LoadModel("Models/Equipo/anilloAst.obj");

	planeta1 = Model();
	planeta1.LoadModel("Models/Equipo/planeta1.obj");

	planeta2 = Model();
	planeta2.LoadModel("Models/Equipo/planeta2.obj");

	cuerpoSat = Model();
	cuerpoSat.LoadModel("Models/Equipo/cuerpoSat.obj");

	antenaSat = Model();
	antenaSat.LoadModel("Models/Equipo/antena.obj");

	panelIzqSat = Model();
	panelIzqSat.LoadModel("Models/Equipo/panelIzq.obj");

	panelDerSat = Model();
	panelDerSat.LoadModel("Models/Equipo/panelDer.obj");

	asteroides = Model();
	asteroides.LoadModel("Models/Equipo/asteroides.obj");

	// ----- Configuración Skybox Escenario 1 -----
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Equipo/Skybox/espacio_rt.tga");
	skyboxFaces.push_back("Textures/Equipo/Skybox/espacio_lf.tga");
	skyboxFaces.push_back("Textures/Equipo/Skybox/espacio_up.tga");
	skyboxFaces.push_back("Textures/Equipo/Skybox/espacio_dn.tga");
	skyboxFaces.push_back("Textures/Equipo/Skybox/espacio_bk.tga");
	skyboxFaces.push_back("Textures/Equipo/Skybox/espacio_ft.tga");

	stbi_set_flip_vertically_on_load(false);
	skybox1 = Skybox(skyboxFaces);
	
	// ----- Configuración Skybox Escenario 2 (Planeta 1)-----
	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Equipo/Skybox/planeta1_rt.tga"); // Cambia estas rutas por tus texturas
	skyboxFaces2.push_back("Textures/Equipo/Skybox/planeta1_lf.tga");
	skyboxFaces2.push_back("Textures/Equipo/Skybox/planeta1_up.tga");
	skyboxFaces2.push_back("Textures/Equipo/Skybox/planeta1_dn.tga");
	skyboxFaces2.push_back("Textures/Equipo/Skybox/planeta1_bk.tga");
	skyboxFaces2.push_back("Textures/Equipo/Skybox/planeta1_ft.tga");
	skybox2 = Skybox(skyboxFaces2);
	stbi_set_flip_vertically_on_load(true);
	/*
	// ----- Configuración Skybox Escenario 3 (Planeta 2) -----
	std::vector<std::string> skyboxFaces3;
	skyboxFaces3.push_back("Textures/Equipo/Skybox/espacio_rt.tga"); // Cambia estas rutas por tus texturas
	skyboxFaces3.push_back("Textures/Equipo/Skybox/espacio_lf.tga");
	skyboxFaces3.push_back("Textures/Equipo/Skybox/espacio_up.tga");
	skyboxFaces3.push_back("Textures/Equipo/Skybox/espacio_dn.tga");
	skyboxFaces3.push_back("Textures/Equipo/Skybox/espacio_bk.tga");
	skyboxFaces3.push_back("Textures/Equipo/Skybox/espacio_ft.tga");
	skybox3 = Skybox(skyboxFaces3);
	*/
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//Declaración de primer luz puntual (Modelo del Sol)
	pointLights[0] = PointLight(1.0f, 0.75f, 0.0f,
		5.0f, 5.0f,				// radiacion y color (0.3f y 0.6f)
		0.0f, 75.0f, -150.0f,
		0.3f, 0.0005f, 0.0003f); 
	pointLightCount++;

	//Declaración de primer luz puntual
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		0.8f, 0.8f,				// radiacion y color (0.3f y 0.6f)
		0.0f, 0.0f, 150.0f,
		0.3f, 0.2f, 0.01f);
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

	// luz infinita -> por eso no vemos cambios en lo que ilumina la luz, ya que al estar tan cerca del piso, no cambia la iluminacion
	
	//se crean mas luces puntuales y spotlight 


    stbi_set_flip_vertically_on_load(false);
    skybox3 = Skybox({"Textures/Equipo/Skybox/erid_right.jpg", "Textures/Equipo/Skybox/erid_left.jpg",
                     "Textures/Equipo/Skybox/erid_top.jpg", "Textures/Equipo/Skybox/erid_bottom.jpg",
                     "Textures/Equipo/Skybox/erid_back.jpg", "Textures/Equipo/Skybox/erid_front.jpg"});
    stbi_set_flip_vertically_on_load(true);

}

void dibujarBase(int escenario, float dt, Camera& camera, const glm::mat4& projection) {
    escenarioActual=escenario;
    deltaTime=dt*60.0f; // Las velocidades originales estaban expresadas por frame.
    glClearColor(0.04f,0.04f,0.08f,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(escenario==1) skybox1.DrawSkybox(camera.calculateViewMatrix(),projection);
    else skybox3.DrawSkybox(camera.calculateViewMatrix(),projection);
    mainLight=DirectionalLight(1,1,1,0.45f,0.55f,0,-1,0);
    GLuint uniformProjection=0,uniformModel=0,uniformView=0,uniformEyePosition=0;
    GLuint uniformSpecularIntensity=0,uniformShininess=0,uniformColor=0;
		// Configuración de los Shaders de Iluminación 
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

		
		if (escenarioActual == 1)
		{
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetPointLights(pointLights, 1); // sólo el Sol
			shaderList[0].SetSpotLights(spotLights, 0);
		}
		else if (escenarioActual == 2)
		{
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetPointLights(pointLights, 0);
			shaderList[0].SetSpotLights(spotLights, 0);
		}
		else if (escenarioActual == 3)
		{
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetPointLights(pointLights, 0);
			shaderList[0].SetSpotLights(spotLights, 1); // sólo la linterna
		}

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0); // para la jerarquía del satelite
		glm::mat4 modelaux3(1.0); // para la jerarquía del satelite
		glm::mat4 modelaux4(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		// Dibujado de objetos según el escenario
		if (escenarioActual == 1) {

			// Animación básica de los planetas
			rotaPlanetas += rotaPlanetasOffset * deltaTime;
			if (rotaPlanetas >= 360.0f * toRadians) {
				rotaPlanetas = 0.0f;
			}

			// Animación básica de los anillos de asteroides
			rotaAnillo += rotaAnilloOffset * deltaTime;

			// Animación de la luna orbitando alrededor del planeta 1
			rotaLuna += rotaLunaOffset * deltaTime;

			if (rotaLuna >= 360.0f * toRadians) {
				rotaLuna = 0.0f;
			}

			// Animación del satélite
			rotaAntena += rotaAntenaOffset * deltaTime;
			rotaPanelIzq += rotaPanelIzqOffset * deltaTime;
			rotaPanelDer += rotaPanelDerOffset * deltaTime;
			movSatelite += movSateliteOffset * deltaTime;
			rotacionSatelite += 0.001f * deltaTime;

			// Sol
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 75.0f, -150.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			sol.RenderModel();

			// Planeta 1
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			planeta1.RenderModel();

			// Luna
			model = glm::mat4(1.0f);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, rotaLuna, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(100.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaLuna * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			luna.RenderModel();

			// Satélite
			// Cuerpo del satelite
			model = modelaux2;
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, movSatelite, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(60.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotacionSatelite, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			cuerpoSat.RenderModel();

			// Antena
			model = modelaux;						//  z , x, y
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.75f));
			model = glm::rotate(model, rotaAntena, glm::vec3(0.0f, 0.0f, 1.0f));
			modelaux3 = model;
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			antenaSat.RenderModel();

			// Panel solar 1
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 1.7f, 0.0f));
			model = glm::rotate(model, rotaPanelIzq, glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux3 = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			panelIzqSat.RenderModel();

			// Panel solar 2
			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, -1.65f, 0.0f));
			model = glm::rotate(model, rotaPanelDer, glm::vec3(0.0f, 1.0f, 0.0f));
			modelaux3 = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			panelDerSat.RenderModel();

			// Planeta 2
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, rotaPlanetas, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			planeta2.RenderModel();

			// Cinturón de asteroides 1
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.55f, 1.55f, 1.55f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			anilloAst.RenderModel();

			// Cinturón de asteroides 2
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, 75.0f * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.7f, 1.7f, 1.7f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			anilloAst.RenderModel();

			// Asteroides
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(-100.0f, -50.0f, 750.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, 45.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			//model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			asteroides.RenderModel();

		}
		else if (escenarioActual == 2) {
			// Coloca aquí los modelos, texturas y posiciones para tu Escenario 2

		}
		else if (escenarioActual == 3) {

			// Animación de Rocky saludando
			if (saludo)
			{
				rotBrazoRocky += rotBrazoRockyOffset * deltaTime;

				if (rotBrazoRocky >= 30.0f)
					saludo = false;
			}
			else
			{
				rotBrazoRocky -= rotBrazoRockyOffset * deltaTime;

				if (rotBrazoRocky <= -30.0f)
					saludo = true;
			}
			
			// Dibujar Piso 
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));

			pisoPlaneta2.UseTexture();
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshListModel[2]->RenderMeshModel();

			// Dibujar Rocas alrededor
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f)
;			model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(150.0f, 150.0f, 150.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			rocas1.RenderModel();

			// Dibujar a Rocky
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 1.25f, 0.0f));
			modelaux4 = model;
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			rocky.RenderModel();

			model = modelaux4;
			model = glm::translate(model, glm::vec3(-0.6f, 1.35f, 2.4f));
			model = glm::rotate(model, rotBrazoRocky * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			brazoRocky.RenderModel();
		}


	
}

}
