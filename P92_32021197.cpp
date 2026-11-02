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
#include <cstdint>

// Inicio: vuelo y humo integrados en este main.
// Logica independiente de OpenGL: permite probar vuelo y humo sin ventana.
namespace VueloAcrobatico {
constexpr double pi = 3.14159265358979323846;
constexpr double periodo = 32.0;
constexpr double vidaHumo = 2.0;
constexpr double emisionesPorSegundo = 45.0; // Por cada salida del motor.
constexpr float escalaAvion = 1.5f;
// El OBJ tiene una helice frontal (+Z), no motores en las puntas de las alas.
const glm::vec3 escapes[] = {{-0.53f,-0.18f,1.9f},{0.53f,-0.18f,1.9f}};

inline float suave(float x) { x=glm::clamp(x,0.0f,1.0f); return x*x*(3-2*x); }
struct Pose { glm::vec3 posicion, frente; glm::mat4 modelo; };
inline Pose pose(double tiempo) {
    double ciclo=std::fmod(tiempo,periodo)/periodo;
    float a=static_cast<float>(2*pi*ciclo);
    glm::vec3 p(110*std::sin(a),40+14*std::sin(3*a),-125+65*std::sin(2*a));
    glm::vec3 f=glm::normalize(glm::vec3(110*std::cos(a),42*std::cos(3*a),130*std::cos(2*a)));
    glm::vec3 derecha=glm::normalize(glm::cross(glm::vec3(0,1,0),f));
    glm::vec3 arriba=glm::normalize(glm::cross(f,derecha));
    glm::mat4 m(1);
    m[0]=glm::vec4(derecha,0); m[1]=glm::vec4(arriba,0);
    m[2]=glm::vec4(f,0); m[3]=glm::vec4(p,1);
    // Alabeo en curvas y un tonel completo con inicio/final suaves por vuelta.
    float giro=0.45f*std::sin(2*a)+static_cast<float>(2*pi)*suave(static_cast<float>((ciclo-0.35)/0.25));
    m=glm::rotate(m,giro,glm::vec3(0,0,1));
    return {p,f,glm::scale(m,glm::vec3(escalaAvion))};
}
struct Particula {
    glm::vec3 origen, deriva;
    double nacimiento;
    float semilla;
    int escape;
    glm::vec3 posicion(double tiempo) const {
        float edad=static_cast<float>(tiempo-nacimiento);
        return origen+deriva*edad+glm::vec3(0,0.35f*edad*edad,0);
    }
};
struct Sistema {
    double tiempo=0;
    std::uint64_t siguienteEmision=0;
    std::vector<Particula> humo;
    void actualizar(double dt) {
        if(!(dt>0) || !std::isfinite(dt)) return;
        tiempo+=dt;
        humo.erase(std::remove_if(humo.begin(),humo.end(),[&](const Particula& p){return tiempo-p.nacimiento>=vidaHumo;}),humo.end());
        // Tras una pausa larga no crear particulas cuya vida ya termino.
        auto primera=static_cast<std::uint64_t>((std::max)(0.0,std::floor((tiempo-vidaHumo)*emisionesPorSegundo)+1));
        siguienteEmision=(std::max)(siguienteEmision,primera);
        while(siguienteEmision/emisionesPorSegundo<=tiempo+1e-10) {
            double nacimiento=siguienteEmision/emisionesPorSegundo;
            Pose avion=pose(nacimiento); // Posicion exacta al emitir, no la del ultimo frame.
            for(int i=0;i<2;++i) {
                float semilla=static_cast<float>((siguienteEmision*37+i*53)%101)/101.0f;
                glm::vec3 origen=glm::vec3(avion.modelo*glm::vec4(escapes[i],1));
                glm::vec3 deriva=-avion.frente*2.0f+glm::vec3((semilla-0.5f)*0.6f,1.1f,0);
                humo.push_back({origen,deriva,nacimiento,semilla,i});
            }
            ++siguienteEmision;
        }
    }
};
}
// Fin: vuelo y humo integrados.
// Inicio: pivotes y animacion del holocron y cohete.
namespace HolocronDatos {
inline const glm::vec3 pivotes[8] = {
    glm::vec3(-2.6f, -2.5f, -2.6f),
    glm::vec3(2.6f, -2.5f, -2.6f),
    glm::vec3(-2.6f, 2.6f, -2.5f),
    glm::vec3(2.6f, 2.7f, -2.5f),
    glm::vec3(-2.6, -2.6f, 2.5f),
    glm::vec3(2.5f, -2.6f, 2.5f),
    glm::vec3(-2.6f, 2.5f, 2.6f),
    glm::vec3(2.5f, 2.5f, 2.6f)
};
}

namespace AnimacionNueva {
constexpr double pi = 3.14159265358979323846;
inline double suave(double u) {
    u = glm::clamp(u, 0.0, 1.0);
    return u*u*u*(10.0+u*(-15.0+6.0*u));
}
inline double derivadaSuave(double u) {
    u = glm::clamp(u, 0.0, 1.0);
    return 30*u*u*(1-u)*(1-u);
}
inline glm::mat4 raizHolocron() {
    return glm::scale(glm::translate(glm::mat4(1), glm::vec3(5,12,-25)), glm::vec3(1.2f));
}
inline glm::mat4 esquinaLocal(int i, double tiempo) {
	const double t = std::fmod(tiempo, 12.0);
	double apertura = 0, giro = 0;
	if (t >= 2 && t < 4)
		apertura = suave((t - 2) / 2);
	else if (t >= 4 && t < 8) {
		apertura = 1;
		giro = suave((t - 4) / 4);
	}
	else if (t >= 8 && t < 10)
		apertura = 1 - suave((t - 8) / 2);
	const glm::vec3 p = HolocronDatos::pivotes[i];
	const glm::vec3 eje = glm::normalize(p);

	glm::mat4 m(1);

	// Orbita alrededor del cuerpo.
	// Las esquinas superiores e inferiores giran en sentidos contrarios.
	if (giro > 0 && giro < 1)
		m = glm::rotate(
			m,
			float(2 * pi * giro) * (p.y > 0 ? 1.0f : -1.0f),
			glm::vec3(0, 1, 0)
		);

	// Separacion radial respecto al centro.
	m = glm::translate(m, p + eje * float(5 * apertura));

	// Giro de la pieza sobre su propio eje.
	if (giro > 0 && giro < 1)
		m = glm::rotate(m, float(2 * pi * giro), eje);

	return m;
}

struct Vuelo { glm::vec3 posicion, velocidad; };
inline Vuelo trayectoriaCohete(double tiempo) {
    double t=std::fmod(tiempo,32.0);
    Vuelo v{glm::vec3(-45,-2,15),glm::vec3(0)};
    if (t<2 || t>=30) return v;
    if (t<8) {
        double u=(t-2)/6;
        v.posicion.y+=float(60*suave(u));
        v.velocidad.y=float(10*derivadaSuave(u));
    } else if (t<24) {
        double u=(t-8)/16, a=2*pi*suave(u), da=2*pi*derivadaSuave(u)/16;
        v.posicion+=glm::vec3(65*(std::cos(a)-1),60+20*(1-std::cos(a)),40*std::sin(a));
        v.velocidad=glm::vec3(-65*std::sin(a)*da,20*std::sin(a)*da,40*std::cos(a)*da);
    } else {
        double u=(t-24)/6;
        v.posicion.y+=float(60*(1-suave(u)));
        v.velocidad.y=float(-10*derivadaSuave(u));
    }
    return v;
}
inline glm::mat4 modeloCohete(double tiempo) {
    auto vuelo=trayectoriaCohete(tiempo);
    double t=std::fmod(tiempo,32.0);
    glm::mat4 orientacion(1);
    // El OBJ apunta hacia +Y. Inclinarlo siguiendo la tangente durante el vuelo.
    // Las transiciones se suavizan para despegar y aterrizar verticalmente.
    if (t>=8 && t<24) {
        double a=2*pi*suave((t-8)/16);
        glm::vec3 tangente(-65*std::sin(a),20*std::sin(a),40*std::cos(a));
        float mezcla=float(suave((t-8)/2)*(1-suave((t-22)/2)));
        glm::vec3 arriba=glm::normalize(glm::mix(glm::vec3(0,1,0),glm::normalize(tangente),mezcla));
        glm::vec3 eje=glm::cross(glm::vec3(0,1,0),arriba);
        if (glm::length(eje)>0.000001f)
            orientacion=glm::rotate(orientacion,std::acos(glm::clamp(arriba.y,-1.0f,1.0f)),glm::normalize(eje));
    }
    glm::mat4 m=glm::translate(glm::mat4(1),vuelo.posicion)*orientacion;
    m=glm::scale(m,glm::vec3(1.6f));
    // Centrar X/Z y apoyar el extremo inferior original en el punto de trayectoria.
    return glm::translate(m,glm::vec3(0.012255f,0.140890f,-0.743053f));
}
}
// Fin: pivotes y animacion del holocron y cohete.

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

struct Animacion {
    double xRover = 0.0;
    double giroRuedas = 0.0;
    bool avanza = true;

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
    }

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
Model HolocronBase_M, HolocronEsquinas_M[8], Cohete_M;
double tiempoNuevaAnimacion = 0;
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





namespace HumoAvion {
VueloAcrobatico::Sistema sistema;
Shader shader;
Texture textura("Textures/smoke.png");
MeshModel quad;
GLint uModel, uView, uProjection, uAge, uSeed;
bool iniciar() {
    shader.CreateFromFiles("shaders/smoke.vert","shaders/smoke.frag");
    shader.UseShader();
    GLint programa=0, enlazado=0;
    glGetIntegerv(GL_CURRENT_PROGRAM,&programa);
    if(programa) glGetProgramiv(programa,GL_LINK_STATUS,&enlazado);
    if(!enlazado) return false;
    uModel=glGetUniformLocation(programa,"model");
    uView=glGetUniformLocation(programa,"view");
    uProjection=glGetUniformLocation(programa,"projection");
    uAge=glGetUniformLocation(programa,"age");
    uSeed=glGetUniformLocation(programa,"seed");
    glUniform1i(glGetUniformLocation(programa,"smokeTexture"),0);
    if(!textura.LoadTextureA()) return false;
    textura.UseTexture();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    GLfloat vertices[]={-0.5f,-0.5f,0, 0,0, 0,0,1,
                         0.5f,-0.5f,0, 1,0, 0,0,1,
                         0.5f, 0.5f,0, 1,1, 0,0,1,
                        -0.5f, 0.5f,0, 0,1, 0,0,1};
    unsigned int indices[]={0,1,2,0,2,3};
    quad.CreateMeshModel(vertices,indices,32,6);
    glUseProgram(0);
    return true;
}
void dibujar(const glm::mat4& view,const glm::mat4& projection) {
    shader.UseShader(); textura.UseTexture();
    glUniformMatrix4fv(uView,1,GL_FALSE,glm::value_ptr(view));
    glUniformMatrix4fv(uProjection,1,GL_FALSE,glm::value_ptr(projection));
    std::vector<const VueloAcrobatico::Particula*> orden;
    for(const auto& p:sistema.humo) orden.push_back(&p);
    // Transparencias de atras hacia adelante, conservando la prueba de profundidad.
    std::sort(orden.begin(),orden.end(),[&](const auto* a,const auto* b) {
        return (view*glm::vec4(a->posicion(sistema.tiempo),1)).z <
               (view*glm::vec4(b->posicion(sistema.tiempo),1)).z;
    });
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    GLboolean culling=glIsEnabled(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    glm::mat4 inversa=glm::inverse(view);
    for(const auto* p:orden) {
        float edad=static_cast<float>((std::max)(0.0,sistema.tiempo-p->nacimiento));
        glm::mat4 m(1);
        // Billboard: el plano siempre mira hacia la camara.
        m[0]=glm::vec4(glm::vec3(inversa[0]),0);
        m[1]=glm::vec4(glm::vec3(inversa[1]),0);
        m[2]=glm::vec4(glm::vec3(inversa[2]),0);
        m[3]=glm::vec4(p->posicion(sistema.tiempo),1);
        m=glm::rotate(m,p->semilla*6.2831853f+edad*(p->semilla-0.5f),glm::vec3(0,0,1));
        float tamano=0.8f+edad*2.6f;
        m=glm::scale(m,glm::vec3(tamano,tamano*1.35f,1));
        glUniformMatrix4fv(uModel,1,GL_FALSE,glm::value_ptr(m));
        glUniform1f(uAge,edad); glUniform1f(uSeed,p->semilla);
        quad.RenderMeshModel();
    }
    if(culling) glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE); glDisable(GL_BLEND);
}
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	if(!HumoAvion::iniciar()) { printf("No se pudo inicializar el humo.\n"); return 1; }
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
    Cohete_M.LoadModel("Models/cohete.obj");


    HolocronBase_M.LoadModel("Models/HolocronJerarquizado/base.obj");

	HolocronEsquinas_M[0].LoadModel("Models/HolocronJerarquizado/esquina_" + std::to_string(0) + ".obj");
	HolocronEsquinas_M[1].LoadModel("Models/HolocronJerarquizado/esquina_" + std::to_string(1) + ".obj");
	HolocronEsquinas_M[2].LoadModel("Models/HolocronJerarquizado/esquina_" + std::to_string(2) + ".obj");
	HolocronEsquinas_M[3].LoadModel("Models/HolocronJerarquizado/esquina_" + std::to_string(3) + ".obj");
	HolocronEsquinas_M[4].LoadModel("Models/HolocronJerarquizado/esquina_" + std::to_string(4) + ".obj");
	HolocronEsquinas_M[5].LoadModel("Models/HolocronJerarquizado/esquina_" + std::to_string(5) + ".obj");
	HolocronEsquinas_M[6].LoadModel("Models/HolocronJerarquizado/esquina_" + std::to_string(6) + ".obj");
	HolocronEsquinas_M[7].LoadModel("Models/HolocronJerarquizado/esquina_" + std::to_string(7) + ".obj");



	

	// Cubemap de seis caras del valle helado (ajuste de altura incluido).
    stbi_set_flip_vertically_on_load(false);
    std::vector<std::string> carasValle;
    for (const char* cara : { "px", "nx", "py", "ny", "pz", "nz" })
        carasValle.push_back(std::string("Textures/Skybox/valle_helado_cubemap/") + cara + ".png");
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    skybox = Skybox(carasValle);
    stbi_set_flip_vertically_on_load(true);

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
        tiempoNuevaAnimacion += segundosFrame;
        animacion.actualizar(segundosFrame);
        HumoAvion::sistema.actualizar(segundosFrame);
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

        // Holocron: padre comun y ocho hijas con pivote propio.
        const glm::mat4 raizHolocron=AnimacionNueva::raizHolocron();
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(raizHolocron));
        glUniform3f(uniformColor,0.12f,0.65f,1.0f);
        Material_brillante.UseMaterial(uniformSpecularIntensity,uniformShininess);
        HolocronBase_M.RenderModel();
        glUniform3f(uniformColor,0.85f,0.59f,0.22f);
        for (int i=0;i<8;++i) {
            model=raizHolocron*AnimacionNueva::esquinaLocal(i,tiempoNuevaAnimacion);
            glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
            HolocronEsquinas_M[i].RenderModel();
        }
        glUniform3f(uniformColor,0.88f,0.91f,0.95f);
        model=AnimacionNueva::modeloCohete(tiempoNuevaAnimacion);
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
        Cohete_M.RenderModel();
        glUniform3f(uniformColor,1,1,1);

        // Vuelo del avion
        model=VueloAcrobatico::pose(HumoAvion::sistema.tiempo).modelo;
        glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));
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

		HumoAvion::dibujar(camera.calculateViewMatrix(),projection);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
