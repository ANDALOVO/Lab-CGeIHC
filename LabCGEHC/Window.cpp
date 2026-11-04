#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	avanceRover = 0.0f;
	avanceAvion = 0.0f;
	alturaAvion = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	avanceRover = 0.0f;
	avanceAvion = 0.0f;
	alturaAvion = 0.0f;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise(bool visible)
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_VISIBLE, visible ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto final | 1 Espacio | 2 Valle helado | 3 Erid", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
	return 0;
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	// Solo PRESS: ignorar REPEAT y RELEASE para cambiar una vez por pulsacion.
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_N)
			theWindow->lucesNaveEncendidas = !theWindow->lucesNaveEncendidas;
		if (key == GLFW_KEY_O)
			theWindow->motoresIzquierdosEncendidos = !theWindow->motoresIzquierdosEncendidos;
		if (key == GLFW_KEY_P)
			theWindow->motoresDerechosEncendidos = !theWindow->motoresDerechosEncendidos;
		if (key == GLFW_KEY_Z)
			theWindow->linternaEncendida = !theWindow->linternaEncendida;
		if (key == GLFW_KEY_C)
			theWindow->colorFaroAvion = (theWindow->colorFaroAvion + 1) % 6;
		if (key == GLFW_KEY_V)
			theWindow->luzHolocronEncendida = !theWindow->luzHolocronEncendida;
		if (key == GLFW_KEY_B)
			theWindow->luzPinzaEncendida = !theWindow->luzPinzaEncendida;
	}

	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}

	// Movimiento continuo de los modelos mientras se mantengan presionadas las teclas.
	// Rover: G adelante, F atras. Avion: H adelante, J atras, I sube, K baja.
	theWindow->avanceRover = (theWindow->keys[GLFW_KEY_G] ? 1.0f : 0.0f)
		- (theWindow->keys[GLFW_KEY_F] ? 1.0f : 0.0f);
	theWindow->avanceAvion = (theWindow->keys[GLFW_KEY_H] ? 1.0f : 0.0f)
		- (theWindow->keys[GLFW_KEY_J] ? 1.0f : 0.0f);
	theWindow->alturaAvion = (theWindow->keys[GLFW_KEY_I] ? 1.0f : 0.0f)
		- (theWindow->keys[GLFW_KEY_K] ? 1.0f : 0.0f);
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
