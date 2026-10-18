#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	// Direccion de movimiento de los modelos: 1 = adelante/arriba, -1 = atras/abajo, 0 = quieto
	GLfloat getavanceRover() { return avanceRover; }   // G adelante, F atras
	GLfloat getavanceAvion() { return avanceAvion; }   // H adelante, J atras
	GLfloat getalturaAvion() { return alturaAvion; }   // I sube, K baja
	int getColorFaroAvion() const { return colorFaroAvion; }
	bool getLuzHolocronEncendida() const { return luzHolocronEncendida; }
	bool getLuzPinzaEncendida() const { return luzPinzaEncendida; }
	bool getLinternaEncendida() const { return linternaEncendida; }
	bool getLucesNaveEncendidas() const { return lucesNaveEncendidas; }
	bool getMotoresIzquierdosEncendidos() const { return motoresIzquierdosEncendidos; }
	bool getMotoresDerechosEncendidos() const { return motoresDerechosEncendidos; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat avanceRover;
	GLfloat avanceAvion;
	GLfloat alturaAvion;
	bool mouseFirstMoved;
	// -1 conserva el blanco inicial; la primera C selecciona rojo.
	int colorFaroAvion = -1;
	bool luzHolocronEncendida = true;
	bool luzPinzaEncendida = true;
	bool linternaEncendida = true;
	bool lucesNaveEncendidas = true; // N: interruptor general, conserva estados O/P
	bool motoresIzquierdosEncendidos = true; // O: los dos motores de X negativo
	bool motoresDerechosEncendidos = true;   // P: los dos motores de X positivo
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

