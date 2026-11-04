/*
Proyecto final
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
//#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminaciÃ³n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
// Implementacion integrada para la entrega del equipo.
// Los encabezados originales se conservan para poder separar el codigo despues.
// ===== Integracion de CrepperDatos.h =====
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>

namespace CrepperDatos {
struct Nodo { const char* nombre; const char* archivo; int padre; glm::vec3 traslacion; };
inline const Nodo nodos[] = {
    {"Crepper_Raiz", nullptr, -1, glm::vec3(0.0f, 0.0f, 0.0f)},
    {"cuerpo_inferior", "Models/CrepperJerarquizado/cuerpo_inferior.obj", 0, glm::vec3(0.0f, 0.67f, 0.05f)},
    {"cabeza", "Models/CrepperJerarquizado/cabeza.obj", 1, glm::vec3(0.0f, 0.24f, -1.44f)},
    {"borde_frontal", "Models/CrepperJerarquizado/borde_frontal.obj", 1, glm::vec3(0.000000000f, 0.000000000f, -1.975000000f)},
    {"placa_01", "Models/CrepperJerarquizado/placa_01.obj", 1, glm::vec3(0.0f, 0.000000000f, -1.660000000f)},
    {"placa_02", "Models/CrepperJerarquizado/placa_02.obj", 1, glm::vec3(0.0f, 0.000000000f, -1.010000000f)},
    {"placa_03", "Models/CrepperJerarquizado/placa_03.obj", 1, glm::vec3(0.000000000f, 0.000000000f, -0.360000000f)},
    {"placa_04", "Models/CrepperJerarquizado/placa_04.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 0.275000000f)},
    {"placa_05", "Models/CrepperJerarquizado/placa_05.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 0.875000000f)},
    {"placa_06", "Models/CrepperJerarquizado/placa_06.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 1.430000000f)},
    {"placa_07", "Models/CrepperJerarquizado/placa_07.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 1.915000000f)},
    {"cola", "Models/CrepperJerarquizado/cola.obj", 1, glm::vec3(0.000000000f, 0.000000000f, 1.780000000f)},
    {"pata_izquierda_1", "Models/CrepperJerarquizado/pata_izquierda_1.obj", 1, glm::vec3(-0.860000000f, -0.099394742f, -1.510781433f)},
    {"pata_izquierda_2", "Models/CrepperJerarquizado/pata_izquierda_2.obj", 1, glm::vec3(-0.980000000f, -0.099394742f, -1.010781433f)},
    {"pata_izquierda_3", "Models/CrepperJerarquizado/pata_izquierda_3.obj", 1, glm::vec3(-1.020000000f, -0.099394742f, -0.420781433f)},
    {"pata_izquierda_4", "Models/CrepperJerarquizado/pata_izquierda_4.obj", 1, glm::vec3(-1.000000000f, -0.099394742f, 0.179218567f)},
    {"pata_izquierda_5", "Models/CrepperJerarquizado/pata_izquierda_5.obj", 1, glm::vec3(-0.940000000f, -0.099394742f, 0.779218567f)},
    {"pata_izquierda_6", "Models/CrepperJerarquizado/pata_izquierda_6.obj", 1, glm::vec3(-0.820000000f, -0.099394742f, 1.329218567f)},
    {"pata_izquierda_7", "Models/CrepperJerarquizado/pata_izquierda_7.obj", 1, glm::vec3(-0.650000000f, -0.099394742f, 1.799218567f)},
    {"apendice_exterior_izquierda", "Models/CrepperJerarquizado/apendice_exterior_izquierda.obj", 2, glm::vec3(-0.560000000f, -0.435932314f, -0.492195686f)},
    {"apendice_central_izquierda", "Models/CrepperJerarquizado/apendice_central_izquierda.obj", 2, glm::vec3(-0.230000000f, -0.405409379f, -0.711704545f)},
    {"ojo_izquierda", "Models/CrepperJerarquizado/ojo_izquierda.obj", 2, glm::vec3(-0.580000000f, 0.280000000f, -0.836000000f)},
    {"pata_derecha_1", "Models/CrepperJerarquizado/pata_derecha_1.obj", 1, glm::vec3(0.860000000f, -0.099394742f, -1.510781433f)},
    {"pata_derecha_2", "Models/CrepperJerarquizado/pata_derecha_2.obj", 1, glm::vec3(0.980000000f, -0.099394742f, -1.010781433f)},
    {"pata_derecha_3", "Models/CrepperJerarquizado/pata_derecha_3.obj", 1, glm::vec3(1.020000000f, -0.099394742f, -0.420781433f)},
    {"pata_derecha_4", "Models/CrepperJerarquizado/pata_derecha_4.obj", 1, glm::vec3(1.000000000f, -0.099394742f, 0.179218567f)},
    {"pata_derecha_5", "Models/CrepperJerarquizado/pata_derecha_5.obj", 1, glm::vec3(0.940000000f, -0.099394742f, 0.779218567f)},
    {"pata_derecha_6", "Models/CrepperJerarquizado/pata_derecha_6.obj", 1, glm::vec3(0.820000000f, -0.099394742f, 1.329218567f)},
    {"pata_derecha_7", "Models/CrepperJerarquizado/pata_derecha_7.obj", 1, glm::vec3(0.650000000f, -0.099394742f, 1.799218567f)},
    {"apendice_exterior_derecha", "Models/CrepperJerarquizado/apendice_exterior_derecha.obj", 2, glm::vec3(0.560000000f, -0.424067686f, -0.492195686f)},
    {"apendice_central_derecha", "Models/CrepperJerarquizado/apendice_central_derecha.obj", 2, glm::vec3(0.230000000f, -0.405409379f, -0.711704545f)},
    {"ojo_derecha", "Models/CrepperJerarquizado/ojo_derecha.obj", 2, glm::vec3(0.580000000f, 0.280000000f, -0.836000000f)},
};
inline constexpr int cantidad = sizeof(nodos) / sizeof(nodos[0]);
using Matrices = std::array<glm::mat4, cantidad>;
// rotaciones: matrices locales inicializadas a identidad. La raiz controla todo el animal.
inline Matrices calcularMatrices(const glm::mat4& mundo, const Matrices& rotaciones) {
    Matrices resultado;
    for (int i = 0; i < cantidad; ++i) {
        const glm::mat4 padre = nodos[i].padre < 0 ? mundo : resultado[nodos[i].padre];
        resultado[i] = padre * glm::translate(glm::mat4(1.0f), nodos[i].traslacion) * rotaciones[i];
    }
    return resultado;
}
} // namespace CrepperDatos

// ===== Integracion de CrepperNPC.h =====
#include <cmath>
#include <cstring>
#include <algorithm>

// Aportacion de Angel: logica independiente de OpenGL.
namespace CrepperNPC {
constexpr double pi=3.14159265358979323846;
constexpr float radioRecorrido=40, velocidad=6, escala=3;
constexpr float limiteCamara=170;
struct Pose { glm::vec3 posicion, frente; CrepperDatos::Matrices matrices; };
inline Pose evaluar(double tiempo) {
    double angulo=std::fmod(tiempo*velocidad/radioRecorrido,2*pi);
    double paso=tiempo*2*pi*1.3;
    glm::vec3 posicion(radioRecorrido*std::sin(angulo),.22f+.035f*std::sin(2*paso),
                      -40+radioRecorrido*std::cos(angulo));
    glm::vec3 frente(std::cos(angulo),0,-std::sin(angulo));
    auto mundo=glm::translate(glm::mat4(1),posicion);
    mundo=glm::rotate(mundo,std::atan2(-frente.x,-frente.z),glm::vec3(0,1,0));
    mundo=glm::scale(mundo,glm::vec3(escala));
    CrepperDatos::Matrices rotaciones;
    for(int i=0;i<CrepperDatos::cantidad;++i) {
        rotaciones[i]=glm::mat4(1);
        const char* nombre=CrepperDatos::nodos[i].nombre;
        if(std::strncmp(nombre,"pata_",5)==0) {
            int numero=nombre[std::strlen(nombre)-1]-'1';
            bool derecha=std::strstr(nombre,"derecha")!=nullptr;
            float fase=static_cast<float>(paso-numero*.72+(derecha?pi:0));
            rotaciones[i]=glm::rotate(glm::mat4(1),.18f*std::sin(fase),glm::vec3(1,0,0));
            rotaciones[i]=glm::rotate(rotaciones[i],(derecha?-1:1)*.10f*(std::max)(0.f,std::sin(fase)),glm::vec3(0,0,1));
        } else if(std::strcmp(nombre,"cabeza")==0)
            rotaciones[i]=glm::rotate(glm::mat4(1),.045f*std::sin(static_cast<float>(tiempo)),glm::vec3(0,1,0));
        else if(std::strncmp(nombre,"apendice_",9)==0)
            rotaciones[i]=glm::rotate(glm::mat4(1),.07f*std::sin(static_cast<float>(paso)*.5f+i),glm::vec3(1,0,0));
        else if(std::strcmp(nombre,"cola")==0)
            rotaciones[i]=glm::rotate(glm::mat4(1),.04f*std::sin(static_cast<float>(paso)),glm::vec3(0,1,0));
    }
    return {posicion,frente,CrepperDatos::calcularMatrices(mundo,rotaciones)};
}
inline glm::vec3 limitar(glm::vec3 p) {
    float radio=glm::length(glm::vec2(p.x,p.z));
    if(radio>limiteCamara) { p.x*=limiteCamara/radio; p.z*=limiteCamara/radio; }
    p.y=std::clamp(p.y,2.f,65.f);
    return p;
}
}

// ===== KEYFRAMES DEL COHETE (practica10keyframes) =====
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
namespace CoheteKeyframes {
const int piezas = 3;
const char* nombres[piezas] = {"etapa_1", "etapa_2", "capsula"};

///////////////////////////////KEYFRAMES/////////////////////

// Movimiento actual de las etapas 1, 2 y de la capsula con antena.
float movCohete_x[piezas]{}, movCohete_y[piezas]{}, movCohete_z[piezas]{};
float giroCohete_x[piezas]{}, giroCohete_y[piezas]{}, giroCohete_z[piezas]{};
int padreCohete[piezas]{};

#define MAX_FRAMES 100
int i_max_steps = 30;
int i_curr_steps = 0;
typedef struct _frame
{
    float movCohete_x[piezas], movCohete_y[piezas], movCohete_z[piezas];
    float giroCohete_x[piezas], giroCohete_y[piezas], giroCohete_z[piezas];
    int padre[piezas];
    float movCohete_xInc[piezas], movCohete_yInc[piezas], movCohete_zInc[piezas];
    float giroCohete_xInc[piezas], giroCohete_yInc[piezas], giroCohete_zInc[piezas];
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 61;
bool play = false;
int playIndex = 0;
int inicioOrbita = 36;
int seleccion = 0;
int reproduciranimacion = 0, habilitaranimacion = 0;
int guardoFrame = 0, reinicioFrame = 0, ciclo = 0;
int cambioSeleccion = 0, cargoFrame = 0, exportoFrame = 0, buscoFrame = 0;
const char* archivoOriginal = "Animaciones/cohete_keyframes.txt";
const char* archivoEditado = "Animaciones/cohete_editados.txt";

bool guardarSecuencia(const char* ruta, int cantidad);
void aplicarFrame(int indice);

void saveFrame(void) // tecla L
{
    if (FrameIndex >= MAX_FRAMES) {
        printf("No se pueden guardar mas de %d keyframes.\n", MAX_FRAMES);
        return;
    }
    for (int i = 0; i < piezas; ++i) {
        KeyFrame[FrameIndex].movCohete_x[i] = movCohete_x[i];
        KeyFrame[FrameIndex].movCohete_y[i] = movCohete_y[i];
        KeyFrame[FrameIndex].movCohete_z[i] = movCohete_z[i];
        KeyFrame[FrameIndex].giroCohete_x[i] = giroCohete_x[i];
        KeyFrame[FrameIndex].giroCohete_y[i] = giroCohete_y[i];
        KeyFrame[FrameIndex].giroCohete_z[i] = giroCohete_z[i];
        KeyFrame[FrameIndex].padre[i] = padreCohete[i];
    }
    if (!guardarSecuencia(archivoEditado, FrameIndex + 1)) return;
    printf("frameindex %d guardado en %s\n", FrameIndex, archivoEditado);
    FrameIndex++;
}

void resetElements(void) // tecla 0
{
    aplicarFrame(0);
    playIndex = 0;
    i_curr_steps = 0;
}

void interpolation(void)
{
    FRAME& desde = KeyFrame[playIndex];
    const FRAME& hasta = KeyFrame[playIndex + 1];
    for (int i = 0; i < piezas; ++i) {
        desde.movCohete_xInc[i] = (hasta.movCohete_x[i] - desde.movCohete_x[i]) / i_max_steps;
        desde.movCohete_yInc[i] = (hasta.movCohete_y[i] - desde.movCohete_y[i]) / i_max_steps;
        desde.movCohete_zInc[i] = (hasta.movCohete_z[i] - desde.movCohete_z[i]) / i_max_steps;
        desde.giroCohete_xInc[i] = (hasta.giroCohete_x[i] - desde.giroCohete_x[i]) / i_max_steps;
        desde.giroCohete_yInc[i] = (hasta.giroCohete_y[i] - desde.giroCohete_y[i]) / i_max_steps;
        desde.giroCohete_zInc[i] = (hasta.giroCohete_z[i] - desde.giroCohete_z[i]) / i_max_steps;
    }
}

void animate(void)
{
    if (play) {
        if (i_curr_steps >= i_max_steps) {
            playIndex++;
            printf("playindex : %d\n", playIndex);
            if (playIndex > FrameIndex - 2) {
                // En este ejercicio el tramo final se repite en orbita.
                playIndex = inicioOrbita;
            }
            aplicarFrame(playIndex);
            i_curr_steps = 0;
            interpolation();
        } else {
            for (int i = 0; i < piezas; ++i) {
                movCohete_x[i] += KeyFrame[playIndex].movCohete_xInc[i];
                movCohete_y[i] += KeyFrame[playIndex].movCohete_yInc[i];
                movCohete_z[i] += KeyFrame[playIndex].movCohete_zInc[i];
                giroCohete_x[i] += KeyFrame[playIndex].giroCohete_xInc[i];
                giroCohete_y[i] += KeyFrame[playIndex].giroCohete_yInc[i];
                giroCohete_z[i] += KeyFrame[playIndex].giroCohete_zInc[i];
            }
            i_curr_steps++;
        }
    }
}

///////////////* FIN KEYFRAMES*////////////////////////////

void aplicarFrame(int indice)
{
    for (int i = 0; i < piezas; ++i) {
        movCohete_x[i] = KeyFrame[indice].movCohete_x[i];
        movCohete_y[i] = KeyFrame[indice].movCohete_y[i];
        movCohete_z[i] = KeyFrame[indice].movCohete_z[i];
        giroCohete_x[i] = KeyFrame[indice].giroCohete_x[i];
        giroCohete_y[i] = KeyFrame[indice].giroCohete_y[i];
        giroCohete_z[i] = KeyFrame[indice].giroCohete_z[i];
        padreCohete[i] = KeyFrame[indice].padre[i];
    }
}



void iniciarReproduccion() {
    if (FrameIndex < 2) return;
    resetElements();
    interpolation();
    play = true;
    printf("COHETE: REPRODUCIENDO desde keyframe 0; %d pasos por tramo\n", i_max_steps);
}
bool guardarSecuencia(const char* ruta, int cantidad) {
    std::string temporal = std::string(ruta) + ".tmp";
    std::ofstream out(temporal);
    out << "COHETE_KEYFRAMES 3\n" << cantidad << ' ' << inicioOrbita << ' ' << i_max_steps
        << "\n# indice | etapa_1 etapa_2 capsula: padre x y z rotX rotY rotZ\n";
    out << std::setprecision(9);
    for (int n = 0; n < cantidad; ++n) {
        const FRAME& f = KeyFrame[n];
        out << n;
        for (int i = 0; i < piezas; ++i) {
            out << ' ' << f.padre[i] << ' ' << f.movCohete_x[i] << ' ' << f.movCohete_y[i]
                << ' ' << f.movCohete_z[i] << ' ' << f.giroCohete_x[i] << ' ' << f.giroCohete_y[i]
                << ' ' << f.giroCohete_z[i];
        }
        out << '\n';
    }
    out.close();
    if (!out) { printf("No se pudo escribir %s\n", temporal.c_str()); return false; }
    std::error_code error;
    std::filesystem::rename(temporal, ruta, error);
    if (error) { printf("No se pudo actualizar %s: %s\n", ruta, error.message().c_str()); return false; }
    printf("COHETE: %d keyframes GUARDADOS en %s\n", cantidad, ruta);
    return true;
}
bool guardar(const char* ruta) { return guardarSecuencia(ruta, FrameIndex); }
bool cargar(const char* ruta) {
    std::ifstream in(ruta);
    std::string firma, linea;
    int version, cantidad, orbita = -1, pasos = 30;
    double tiempoOrbita = 0, tiempoAnterior = -1;
    if (!(in >> firma >> version >> cantidad) || firma != "COHETE_KEYFRAMES" ||
        (version != 3 && version != 2) || cantidad < 2 || cantidad > MAX_FRAMES) {
        printf("COHETE: TXT invalido o incompatible: %s\n", ruta); return false;
    }
    if (version == 2) {
        if (!(in >> tiempoOrbita) || !std::isfinite(tiempoOrbita)) return false;
    } else if (!(in >> orbita >> pasos) || pasos < 1 || pasos > 600) return false;
    FRAME nuevos[MAX_FRAMES]{};
    int leidos = 0;
    std::getline(in, linea);
    while (std::getline(in, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        if (leidos >= cantidad) return false;
        std::istringstream fila(linea); int indice;
        if (version == 2) {
            double t;
            if (!(fila >> t) || !std::isfinite(t) || t < 0 || t <= tiempoAnterior) return false;
            if (orbita < 0 && t >= tiempoOrbita) orbita = leidos;
            tiempoAnterior = t;
        } else if (!(fila >> indice) || indice != leidos) return false;
        for (int i = 0; i < piezas; ++i) {
            FRAME& f = nuevos[leidos];
            if (!(fila >> f.padre[i] >> f.movCohete_x[i] >> f.movCohete_y[i] >> f.movCohete_z[i]
                       >> f.giroCohete_x[i] >> f.giroCohete_y[i] >> f.giroCohete_z[i])
                || f.padre[i] < -1 || f.padre[i] >= i) return false;
            for (float v : {f.movCohete_x[i], f.movCohete_y[i], f.movCohete_z[i],
                            f.giroCohete_x[i], f.giroCohete_y[i], f.giroCohete_z[i]})
                if (!std::isfinite(v)) return false;
        }
        std::string extra; if (fila >> extra) return false;
        leidos++;
    }
    if (leidos != cantidad || orbita < 0 || orbita >= cantidad - 1) return false;
    for (int i = 0; i < cantidad; ++i) KeyFrame[i] = nuevos[i];
    FrameIndex = cantidad; inicioOrbita = orbita; i_max_steps = pasos;
    resetElements(); interpolation(); play = false;
    printf("COHETE: %d keyframes CARGADOS de %s\n", FrameIndex, ruta);
    return true;
}
std::array<glm::mat4,piezas> matrices() {
    std::array<glm::mat4,piezas> resultado;
    for (int i = 0; i < piezas; ++i) {
        int padre = padreCohete[i];
        glm::mat4 model(1);
        glm::vec3 pos(movCohete_x[i], movCohete_y[i], movCohete_z[i]);
        if (padre >= 0) {
            model = resultado[padre];
            pos -= glm::vec3(movCohete_x[padre], movCohete_y[padre], movCohete_z[padre]);
        }
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(giroCohete_x[i]), {1,0,0});
        model = glm::rotate(model, glm::radians(giroCohete_y[i]), {0,1,0});
        resultado[i] = glm::rotate(model, glm::radians(giroCohete_z[i]), {0,0,1});
    }
    return resultado;
}
void inputKeyframes(bool* keys) {
    // Contadores de habilitacion como en practica10keyframes.cpp.
    if (keys[GLFW_KEY_SPACE] && reproduciranimacion < 1) {
        if (FrameIndex > 1) play = !play;
        if (play && i_curr_steps == 0) {
            if (playIndex == FrameIndex - 1) playIndex = inicioOrbita;
            aplicarFrame(playIndex); interpolation();
        }
        reproduciranimacion++;
        printf("COHETE: %s\n", play ? "REPRODUCIENDO" : "PAUSA");
    }
    if (!keys[GLFW_KEY_SPACE]) reproduciranimacion = 0;
    if ((keys[GLFW_KEY_0] || keys[GLFW_KEY_KP_0]) && habilitaranimacion < 1) {
        iniciarReproduccion(); habilitaranimacion++;
    }
    if (!keys[GLFW_KEY_0] && !keys[GLFW_KEY_KP_0]) habilitaranimacion = 0;
    if (keys[GLFW_KEY_L] && guardoFrame < 1 && !play) {
        saveFrame(); guardoFrame++; reinicioFrame = 0;
        printf("Presiona P para habilitar guardar otro frame\n");
    }
    if (keys[GLFW_KEY_P] && reinicioFrame < 1) {
        guardoFrame = 0; reinicioFrame++;
        printf("Ya puedes guardar otro frame con L\n");
    }
    if (keys[GLFW_KEY_F4] && cambioSeleccion < 1) {
        seleccion = (seleccion + 1) % piezas; cambioSeleccion++;
        printf("COHETE: pieza %s\n", nombres[seleccion]);
    }
    if (!keys[GLFW_KEY_F4]) cambioSeleccion = 0;
    if ((keys[GLFW_KEY_O] || keys[GLFW_KEY_F9]) && cargoFrame < 1) {
        cargar(keys[GLFW_KEY_F9] ? archivoEditado : archivoOriginal); cargoFrame++;
    }
    if (!keys[GLFW_KEY_O] && !keys[GLFW_KEY_F9]) cargoFrame = 0;
    if (keys[GLFW_KEY_F5] && exportoFrame < 1) { guardar(archivoEditado); exportoFrame++; }
    if (!keys[GLFW_KEY_F5]) exportoFrame = 0;
    if (!play) {
        if ((keys[GLFW_KEY_LEFT_BRACKET] || keys[GLFW_KEY_RIGHT_BRACKET]) && buscoFrame < 1) {
            playIndex += keys[GLFW_KEY_RIGHT_BRACKET] ? 1 : -1;
            playIndex = std::clamp(playIndex, 0, FrameIndex - 1);
            aplicarFrame(playIndex); i_curr_steps = 0; buscoFrame++;
        }
        if (ciclo < 1 && (keys[GLFW_KEY_LEFT] || keys[GLFW_KEY_RIGHT] || keys[GLFW_KEY_UP] ||
            keys[GLFW_KEY_DOWN] || keys[GLFW_KEY_PAGE_UP] || keys[GLFW_KEY_PAGE_DOWN] ||
            keys[GLFW_KEY_R] || keys[GLFW_KEY_T])) {
            glm::vec3 cambio(keys[GLFW_KEY_RIGHT]-keys[GLFW_KEY_LEFT],
                keys[GLFW_KEY_PAGE_UP]-keys[GLFW_KEY_PAGE_DOWN],keys[GLFW_KEY_DOWN]-keys[GLFW_KEY_UP]);
            movCohete_x[seleccion] += cambio.x;
            movCohete_y[seleccion] += cambio.y;
            movCohete_z[seleccion] += cambio.z;
            for (int i = seleccion + 1; i < piezas; ++i) {
                int padre = padreCohete[i];
                while (padre >= 0 && padre != seleccion) padre = padreCohete[padre];
                if (padre == seleccion) {
                    movCohete_x[i] += cambio.x; movCohete_y[i] += cambio.y; movCohete_z[i] += cambio.z;
                }
            }
            giroCohete_z[seleccion] += (keys[GLFW_KEY_T]-keys[GLFW_KEY_R])*5;
            i_curr_steps = 0; ciclo++;
            printf("COHETE: pose modificada; M habilita otro cambio, L guarda\n");
        }
    }
    if (!keys[GLFW_KEY_LEFT_BRACKET] && !keys[GLFW_KEY_RIGHT_BRACKET]) buscoFrame = 0;
    if (keys[GLFW_KEY_M]) ciclo = 0;
}
} // CoheteKeyframes: fin de logica independiente de OpenGL

namespace CoheteKeyframes {
Model modelos[piezas];
bool iniciar() {
    for (int i = 0; i < piezas; ++i) {
        modelos[i].LoadModel(std::string("Models/CoheteJerarquizado/")+nombres[i]+".glb");
        if (!modelos[i].cargado()) return false;
    }
    resetElements(); interpolation();
    printf("COHETE: %d keyframes iniciales del main, %d pasos por tramo\n", FrameIndex, i_max_steps);
    printf("2 Valle | ESPACIO play/pausa | 0 reiniciar/play | C camara\n"
        "F4 pieza | flechas X/Z | PgUp/PgDn Y | R/T giro Z | M habilitar otro cambio\n"
        "L agregar pose al final | P habilitar otro guardado | F5 exportar | O cargar original | F9 editados\n");
    return true;
}
void encuadrar(Camera& camera) { camera=Camera({85,65,145},{0,1,0},-116,-1,24,.25f); }
void dibujar(GLint locModel,GLint locTinte,GLint locUsarTex) {
    glUniform1i(locUsarTex,1); glUniform3f(locTinte,1,1,1);
    auto m = matrices();
    for (int i = 0; i < piezas; ++i) {
        glUniformMatrix4fv(locModel,1,GL_FALSE,glm::value_ptr(m[i])); modelos[i].RenderModel();
    }
    char titulo[240];
    snprintf(titulo,sizeof(titulo),"Valle | COHETE %s | keyframe %d/%d | paso %d/%d | ESPACIO pausa | 0 reiniciar | L guardar | P habilitar",
        play ? "PLAY" : "PAUSA",playIndex,FrameIndex-1,i_curr_steps,i_max_steps);
    glfwSetWindowTitle(glfwGetCurrentContext(),titulo);
}
} // CoheteKeyframes

// ===== FIN KEYFRAMES DEL COHETE =====

// ===== Integracion de ValleHelado.h =====
#include <vector>
#include <fstream>
#include <algorithm>
#include <initializer_list>
#include "Texture.h"

// Escenario 2: Valle helado con Crepper NPC y modelos de Planeta 1.
namespace ValleHelado {
// Descomenta esta linea y recompila para volver a cargar, animar y dibujar el Crepper.
// #define MOSTRAR_CREPPER
#ifdef MOSTRAR_CREPPER
inline Model piezas[CrepperDatos::cantidad];
#endif
inline Model torre;
inline Model aurelion;
inline Model explorador;
inline MeshModel terreno;
inline Texture piso;
inline Skybox cielo;
inline Shader shader;
inline double tiempo=0;
inline bool estabaActivo=false;
inline GLint programa=0;
inline const char* vert=R"GLSL(#version 330 core
layout(location=0) in vec3 pos;
layout(location=1) in vec2 tex;
layout(location=2) in vec3 normal;
uniform mat4 model,view,projection;
out vec2 uv;
out vec3 n;
void main(){vec4 p=model*vec4(pos,1); uv=tex;
n=mat3(transpose(inverse(model)))*normal; gl_Position=projection*view*p;})GLSL";
inline const char* frag=R"GLSL(#version 330 core
in vec2 uv;
in vec3 n;
out vec4 salida;
uniform sampler2D texturaPiso;
uniform vec3 tinte;
uniform bool usarTextura;
void main(){
vec3 norm=normalize(n);
float luz=.57+.43*abs(dot(norm,normalize(vec3(.4,1,.25))));
vec3 base=(usarTextura?texture(texturaPiso,uv).rgb:vec3(1))*tinte;
salida=vec4(base*luz,1);
})GLSL";
inline bool cargarModelo(Model& destino, std::initializer_list<const char*> rutas) {
    for(const char* ruta : rutas) {
        std::ifstream prueba(ruta,std::ios::binary);
        if(prueba.good()) {
            destino.LoadModel(ruta);
            return destino.cargado();
        }
    }
    printf("No se encontro el modelo requerido: %s\n", *rutas.begin());
    return false;
}
inline glm::mat4 colocar(const Model& m, glm::vec3 posicion, float altura, float giro=0.f) {
    const auto& a=m.minimo; const auto& b=m.maximo;
    float escala=altura/(std::max)(.001f,b.y-a.y);
    auto matriz=glm::translate(glm::mat4(1),posicion);
    matriz=glm::rotate(matriz,giro,glm::vec3(0,1,0));
    matriz=glm::scale(matriz,glm::vec3(escala));
    return glm::translate(matriz,glm::vec3(-(a.x+b.x)*.5f,-a.y,-(a.z+b.z)*.5f));
}
inline bool iniciar() {
    if(!CoheteKeyframes::iniciar()) return false;
#ifdef MOSTRAR_CREPPER
    for(int i=1;i<CrepperDatos::cantidad;++i) {
        piezas[i].LoadModel(CrepperDatos::nodos[i].archivo);
        if(!piezas[i].cargado()) return false;
    }
#endif
    if(!cargarModelo(torre,{"Models/torre de lanzamiento.glb","Models/Equipo/torre de lanzamiento.glb"})) return false;
    if(!cargarModelo(aurelion,{"Models/aurelion sol.glb","Models/Equipo/aurelion sol.glb"})) return false;
    if(!cargarModelo(explorador,{"Models/carroexplorador.obj","Models/Equipo/carroexplorador.obj"})) return false;
    const char* rutaPiso=nullptr;
    for(const char* candidata:{"Textures/pisoNieve.jpg","Textures/pisoNieve.jpeg","Textures/Equipo/pisoNieve.jpg"}) {
        std::ifstream prueba(candidata,std::ios::binary);
        if(prueba.good()) { rutaPiso=candidata; break; }
    }
    if(!rutaPiso) {
        printf("No se encontro la textura del piso: Textures/pisoNieve.jpg\n");
        return false;
    }
    piso=Texture(rutaPiso);
    if(!piso.LoadTextureA()) return false;
    piso.UseTexture();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
#ifdef GL_TEXTURE_MAX_ANISOTROPY_EXT
    if(GLEW_EXT_texture_filter_anisotropic) {
        GLfloat maxAniso=1.f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&maxAniso);
        if(maxAniso>1.f) glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,(std::min)(maxAniso,8.f));
    }
#endif
    glBindTexture(GL_TEXTURE_2D,0);
    const char* baseCubemap="Textures/Skybox/valle_helado_cubemap/";
    if(!std::ifstream(std::string(baseCubemap)+"px.png",std::ios::binary).good())
        baseCubemap="Textures/valle_helado_cubemap/";
    stbi_set_flip_vertically_on_load(false);
    std::vector<std::string> caras;
    for(const char* c:{"px","nx","py","ny","pz","nz"})
        caras.push_back(std::string(baseCubemap)+c+".png");
    cielo=Skybox(caras);
    stbi_set_flip_vertically_on_load(true);
    shader.CreateFromString(vert,frag); shader.UseShader();
    glGetIntegerv(GL_CURRENT_PROGRAM,&programa);
    constexpr float mitadPiso=1500.f, escalaU=mitadPiso/40.f, escalaV=mitadPiso/60.f;
    GLfloat vertices[]={
        -mitadPiso,0.f,-mitadPiso, -escalaU,-escalaV, 0.f,1.f,0.f,
         mitadPiso,0.f,-mitadPiso,  escalaU,-escalaV, 0.f,1.f,0.f,
        -mitadPiso,0.f, mitadPiso, -escalaU, escalaV, 0.f,1.f,0.f,
         mitadPiso,0.f, mitadPiso,  escalaU, escalaV, 0.f,1.f,0.f,
    };
    unsigned indices[]={0,2,1, 1,2,3};
    terreno.CreateMeshModel(vertices,indices,32,6);
    return programa!=0;
}
inline void entrar(Camera& camera) {
    CoheteKeyframes::encuadrar(camera);
    glfwSetWindowTitle(glfwGetCurrentContext(),"Valle helado | 1 Espacio | 2 Valle | 3 Erid");
}
inline void dibujar(Camera& camera,const glm::mat4& projection,double dt) {
    if(!estabaActivo) entrar(camera);
    estabaActivo=true;
    if(std::isfinite(dt) && dt>0) tiempo+=dt;
    auto pos=camera.getCameraPosition(), limitado=CrepperNPC::limitar(pos);
    if(glm::length(pos-limitado)>.0001f) {
        auto f=camera.getCameraDirection();
        camera=Camera(limitado,{0,1,0},glm::degrees(std::atan2(f.z,f.x)),glm::degrees(std::asin(f.y)),24,.25f);
    }
    glClearColor(.65f,.78f,.87f,1); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    auto view=camera.calculateViewMatrix(); cielo.DrawSkybox(view,projection);
    shader.UseShader();
    GLint locModel=glGetUniformLocation(programa,"model");
    GLint locTinte=glGetUniformLocation(programa,"tinte");
    GLint locUsarTex=glGetUniformLocation(programa,"usarTextura");
    glUniformMatrix4fv(glGetUniformLocation(programa,"view"),1,GL_FALSE,glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(programa,"projection"),1,GL_FALSE,glm::value_ptr(projection));
    glUniform1i(glGetUniformLocation(programa,"texturaPiso"),0);

    // Piso nevado
    auto ident=glm::mat4(1);
    glUniformMatrix4fv(locModel,1,GL_FALSE,glm::value_ptr(ident));
    glUniform3f(locTinte,1.f,1.f,1.f);
    glUniform1i(locUsarTex,1);
    piso.UseTexture();
    terreno.RenderMeshModel();

    // Modelos de Planeta 1 (companeros): torre Hextech, Aurelion Sol y carro explorador
    auto mTorre=colocar(torre,{-58.f,0.f,-55.f},46.f);
    glUniformMatrix4fv(locModel,1,GL_FALSE,glm::value_ptr(mTorre));
    torre.RenderModel();

    auto mAurelion=colocar(aurelion,{-42.f,24.f,-95.f},22.f,static_cast<float>(tiempo*.15));
    glUniformMatrix4fv(locModel,1,GL_FALSE,glm::value_ptr(mAurelion));
    aurelion.RenderModel();

    auto mExplorador=colocar(explorador,{40.f,0.f,-8.f},7.5f,-.65f);
    glUniform3f(locTinte,.76f,.82f,.88f);
    glUniformMatrix4fv(locModel,1,GL_FALSE,glm::value_ptr(mExplorador));
    explorador.RenderModel();

    CoheteKeyframes::dibujar(locModel,locTinte,locUsarTex);

    // Crepper NPC jerarquizado
#ifdef MOSTRAR_CREPPER
    glUniform1i(locUsarTex,0);
    auto pose=CrepperNPC::evaluar(tiempo);
    for(int i=1;i<CrepperDatos::cantidad;++i) {
        const char* nombre=CrepperDatos::nodos[i].nombre;
        glm::vec3 color(.53f,.59f,.58f);
        if(std::strncmp(nombre,"ojo_",4)==0) color={.018f,.025f,.025f};
        else if(std::strcmp(nombre,"cabeza")==0 || std::strncmp(nombre,"apendice",8)==0) color={.65f,.69f,.67f};
        else if(std::strcmp(nombre,"borde_frontal")==0) color={.2f,.28f,.28f};
        glUniform3fv(locTinte,1,glm::value_ptr(color));
        glUniformMatrix4fv(locModel,1,GL_FALSE,glm::value_ptr(pose.matrices[i]));
        piezas[i].RenderModel();
    }
#endif
    glUseProgram(0);
}
inline void captura(const std::string& ruta) {
    int w,h;glfwGetFramebufferSize(glfwGetCurrentContext(),&w,&h);
    std::vector<unsigned char> datos(w*h*3);
    glPixelStorei(GL_PACK_ALIGNMENT,1); glReadPixels(0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,datos.data());
    std::ofstream f(ruta,std::ios::binary);
    f<<"P6\n"<<w<<" "<<h<<"\n255\n";
    for(int y=h-1;y>=0;--y)f.write(reinterpret_cast<char*>(datos.data()+y*w*3),w*3);
}
inline void captura(int numero) {
    captura("x64/Practica9/valle_"+std::to_string(numero)+".ppm");
}
}

// ===== Fin de la integracion del Crepper y valle helado =====

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoPlaneta2;

// AÃ±adir Texturas a usar

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

//AÃ±adir modelo del aviÃ³n. 
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

// Variables para animaciÃ³n


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


//funciÃ³n de calculo de normales por promedio de vÃ©rtices 
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





int main(int argc, char** argv)
{
	const bool grabarCohete = argc > 1 && std::string(argv[1]) == "--grabar-cohete";
	mainWindow = Window(grabarCohete ? 1280 : 1366, grabarCohete ? 720 : 768);
	const bool verificarValle = argc > 1 && std::string(argv[1]) == "--verificar-valle";
	const bool verificarTodo = argc > 1 && std::string(argv[1]) == "--verificar";
	const bool verificar = verificarValle || verificarTodo || grabarCohete;
	if (mainWindow.Initialise(!verificar) != 0) return 1;
	int framePrueba = 0;
	bool falloGL = false;
	CreateObjects();
	CreateShaders();
    // Valores iniciales de los keyframes, como en practica10keyframes.cpp.
    using CoheteKeyframes::KeyFrame;
    // INICIO KEYFRAMES COHETE
    KeyFrame[0].padre[0] = -1;
    KeyFrame[0].movCohete_x[0] = -61.400000f;
    KeyFrame[0].movCohete_y[0] = 13.545328f;
    KeyFrame[0].movCohete_z[0] = -55.000000f;
    KeyFrame[0].giroCohete_x[0] = 0.000000f;
    KeyFrame[0].giroCohete_y[0] = 0.000000f;
    KeyFrame[0].giroCohete_z[0] = 0.000000f;
    KeyFrame[0].padre[1] = 0;
    KeyFrame[0].movCohete_x[1] = -61.400000f;
    KeyFrame[0].movCohete_y[1] = 23.063703f;
    KeyFrame[0].movCohete_z[1] = -55.000000f;
    KeyFrame[0].giroCohete_x[1] = 0.000000f;
    KeyFrame[0].giroCohete_y[1] = 0.000000f;
    KeyFrame[0].giroCohete_z[1] = 0.000000f;
    KeyFrame[0].padre[2] = 1;
    KeyFrame[0].movCohete_x[2] = -61.400000f;
    KeyFrame[0].movCohete_y[2] = 31.548375f;
    KeyFrame[0].movCohete_z[2] = -55.000000f;
    KeyFrame[0].giroCohete_x[2] = 0.000000f;
    KeyFrame[0].giroCohete_y[2] = 0.000000f;
    KeyFrame[0].giroCohete_z[2] = 0.000000f;
    KeyFrame[1] = KeyFrame[0];
    KeyFrame[2] = KeyFrame[1];
    KeyFrame[3] = KeyFrame[2];
    KeyFrame[3].movCohete_y[0] = 15.545328f;
    KeyFrame[3].movCohete_y[1] = 25.063703f;
    KeyFrame[3].movCohete_y[2] = 33.548375f;
    KeyFrame[4] = KeyFrame[3];
    KeyFrame[4].movCohete_y[0] = 17.545328f;
    KeyFrame[4].movCohete_y[1] = 27.063703f;
    KeyFrame[4].movCohete_y[2] = 35.548375f;
    KeyFrame[5] = KeyFrame[4];
    KeyFrame[5].movCohete_y[0] = 19.545328f;
    KeyFrame[5].movCohete_y[1] = 29.063703f;
    KeyFrame[5].movCohete_y[2] = 37.548375f;
    KeyFrame[6] = KeyFrame[5];
    KeyFrame[6].movCohete_y[0] = 21.545328f;
    KeyFrame[6].movCohete_y[1] = 31.063703f;
    KeyFrame[6].movCohete_y[2] = 39.548375f;
    KeyFrame[7] = KeyFrame[6];
    KeyFrame[7].movCohete_y[0] = 25.211995f;
    KeyFrame[7].movCohete_y[1] = 34.730370f;
    KeyFrame[7].movCohete_y[2] = 43.215042f;
    KeyFrame[8] = KeyFrame[7];
    KeyFrame[8].movCohete_y[0] = 28.878661f;
    KeyFrame[8].movCohete_y[1] = 38.397036f;
    KeyFrame[8].movCohete_y[2] = 46.881709f;
    KeyFrame[9] = KeyFrame[8];
    KeyFrame[9].movCohete_y[0] = 32.545328f;
    KeyFrame[9].movCohete_y[1] = 42.063703f;
    KeyFrame[9].movCohete_y[2] = 50.548375f;
    KeyFrame[10] = KeyFrame[9];
    KeyFrame[10].movCohete_y[0] = 36.211995f;
    KeyFrame[10].movCohete_y[1] = 45.730370f;
    KeyFrame[10].movCohete_y[2] = 54.215042f;
    KeyFrame[11] = KeyFrame[10];
    KeyFrame[11].movCohete_y[0] = 39.878661f;
    KeyFrame[11].movCohete_y[1] = 49.397036f;
    KeyFrame[11].movCohete_y[2] = 57.881709f;
    KeyFrame[12] = KeyFrame[11];
    KeyFrame[12].movCohete_y[0] = 43.545328f;
    KeyFrame[12].padre[1] = -1;
    KeyFrame[12].movCohete_y[1] = 53.063703f;
    KeyFrame[12].movCohete_y[2] = 61.548375f;
    KeyFrame[13] = KeyFrame[12];
    KeyFrame[13].movCohete_x[0] = -63.400000f;
    KeyFrame[13].movCohete_y[0] = 44.023809f;
    KeyFrame[13].movCohete_z[0] = -53.750000f;
    KeyFrame[13].giroCohete_z[0] = -10.000000f;
    KeyFrame[13].movCohete_y[1] = 56.813703f;
    KeyFrame[13].movCohete_y[2] = 65.298375f;
    KeyFrame[14] = KeyFrame[13];
    KeyFrame[14].movCohete_x[0] = -65.400000f;
    KeyFrame[14].movCohete_y[0] = 43.792584f;
    KeyFrame[14].movCohete_z[0] = -52.500000f;
    KeyFrame[14].giroCohete_z[0] = -20.000000f;
    KeyFrame[14].movCohete_y[1] = 60.563703f;
    KeyFrame[14].movCohete_y[2] = 69.048375f;
    KeyFrame[15] = KeyFrame[14];
    KeyFrame[15].movCohete_x[0] = -67.400000f;
    KeyFrame[15].movCohete_y[0] = 42.851655f;
    KeyFrame[15].movCohete_z[0] = -51.250000f;
    KeyFrame[15].giroCohete_z[0] = -30.000000f;
    KeyFrame[15].movCohete_y[1] = 64.313703f;
    KeyFrame[15].movCohete_y[2] = 72.798375f;
    KeyFrame[16] = KeyFrame[15];
    KeyFrame[16].movCohete_x[0] = -69.400000f;
    KeyFrame[16].movCohete_y[0] = 41.201020f;
    KeyFrame[16].movCohete_z[0] = -50.000000f;
    KeyFrame[16].giroCohete_z[0] = -40.000000f;
    KeyFrame[16].movCohete_y[1] = 68.063703f;
    KeyFrame[16].movCohete_y[2] = 76.548375f;
    KeyFrame[17] = KeyFrame[16];
    KeyFrame[17].movCohete_x[0] = -71.400000f;
    KeyFrame[17].movCohete_y[0] = 38.840680f;
    KeyFrame[17].movCohete_z[0] = -48.750000f;
    KeyFrame[17].giroCohete_z[0] = -50.000000f;
    KeyFrame[17].movCohete_y[1] = 71.813703f;
    KeyFrame[17].movCohete_y[2] = 80.298375f;
    KeyFrame[18] = KeyFrame[17];
    KeyFrame[18].movCohete_x[0] = -73.400000f;
    KeyFrame[18].movCohete_y[0] = 35.770635f;
    KeyFrame[18].movCohete_z[0] = -47.500000f;
    KeyFrame[18].giroCohete_z[0] = -60.000000f;
    KeyFrame[18].movCohete_y[1] = 75.563703f;
    KeyFrame[18].movCohete_y[2] = 84.048375f;
    KeyFrame[19] = KeyFrame[18];
    KeyFrame[19].movCohete_x[0] = -75.400000f;
    KeyFrame[19].movCohete_y[0] = 31.990884f;
    KeyFrame[19].movCohete_z[0] = -46.250000f;
    KeyFrame[19].giroCohete_z[0] = -70.000000f;
    KeyFrame[19].movCohete_y[1] = 79.313703f;
    KeyFrame[19].movCohete_y[2] = 87.798375f;
    KeyFrame[20] = KeyFrame[19];
    KeyFrame[20].movCohete_x[0] = -77.400000f;
    KeyFrame[20].movCohete_y[0] = 27.501429f;
    KeyFrame[20].movCohete_z[0] = -45.000000f;
    KeyFrame[20].giroCohete_z[0] = -80.000000f;
    KeyFrame[20].movCohete_y[1] = 83.063703f;
    KeyFrame[20].padre[2] = -1;
    KeyFrame[20].movCohete_y[2] = 91.548375f;
    KeyFrame[21] = KeyFrame[20];
    KeyFrame[21].movCohete_x[0] = -79.400000f;
    KeyFrame[21].movCohete_y[0] = 22.302268f;
    KeyFrame[21].movCohete_z[0] = -43.750000f;
    KeyFrame[21].giroCohete_z[0] = -90.000000f;
    KeyFrame[21].movCohete_x[1] = -59.566667f;
    KeyFrame[21].movCohete_y[1] = 83.261566f;
    KeyFrame[21].movCohete_z[1] = -53.916667f;
    KeyFrame[21].giroCohete_z[1] = 10.000000f;
    KeyFrame[21].movCohete_y[2] = 93.735875f;
    KeyFrame[21].giroCohete_z[2] = -1.250000f;
    KeyFrame[22] = KeyFrame[21];
    KeyFrame[22].movCohete_x[0] = -81.400000f;
    KeyFrame[22].movCohete_y[0] = 16.393402f;
    KeyFrame[22].movCohete_z[0] = -42.500000f;
    KeyFrame[22].movCohete_x[1] = -57.733333f;
    KeyFrame[22].movCohete_y[1] = 82.188491f;
    KeyFrame[22].movCohete_z[1] = -52.833333f;
    KeyFrame[22].giroCohete_z[1] = 20.000000f;
    KeyFrame[22].movCohete_y[2] = 95.923375f;
    KeyFrame[22].giroCohete_z[2] = -2.500000f;
    KeyFrame[23] = KeyFrame[22];
    KeyFrame[23].movCohete_x[0] = -83.400000f;
    KeyFrame[23].movCohete_y[0] = 9.774831f;
    KeyFrame[23].movCohete_z[0] = -41.250000f;
    KeyFrame[23].movCohete_x[1] = -55.900000f;
    KeyFrame[23].movCohete_y[1] = 79.844476f;
    KeyFrame[23].movCohete_z[1] = -51.750000f;
    KeyFrame[23].giroCohete_z[1] = 30.000000f;
    KeyFrame[23].movCohete_y[2] = 98.110875f;
    KeyFrame[23].giroCohete_z[2] = -3.750000f;
    KeyFrame[24] = KeyFrame[23];
    KeyFrame[24].movCohete_x[0] = -85.400000f;
    KeyFrame[24].movCohete_y[0] = 2.446555f;
    KeyFrame[24].movCohete_z[0] = -40.000000f;
    KeyFrame[24].movCohete_x[1] = -54.066667f;
    KeyFrame[24].movCohete_y[1] = 76.229521f;
    KeyFrame[24].movCohete_z[1] = -50.666667f;
    KeyFrame[24].giroCohete_z[1] = 40.000000f;
    KeyFrame[24].movCohete_y[2] = 100.298375f;
    KeyFrame[24].giroCohete_z[2] = -5.000000f;
    KeyFrame[25] = KeyFrame[24];
    KeyFrame[25].movCohete_x[1] = -52.233333f;
    KeyFrame[25].movCohete_y[1] = 71.343627f;
    KeyFrame[25].movCohete_z[1] = -49.583333f;
    KeyFrame[25].giroCohete_z[1] = 50.000000f;
    KeyFrame[25].movCohete_y[2] = 102.485875f;
    KeyFrame[25].giroCohete_z[2] = -6.250000f;
    KeyFrame[26] = KeyFrame[25];
    KeyFrame[26].movCohete_x[1] = -50.400000f;
    KeyFrame[26].movCohete_y[1] = 65.186794f;
    KeyFrame[26].movCohete_z[1] = -48.500000f;
    KeyFrame[26].giroCohete_z[1] = 60.000000f;
    KeyFrame[26].movCohete_y[2] = 104.673375f;
    KeyFrame[26].giroCohete_z[2] = -7.500000f;
    KeyFrame[27] = KeyFrame[26];
    KeyFrame[27].movCohete_x[1] = -48.566667f;
    KeyFrame[27].movCohete_y[1] = 57.759022f;
    KeyFrame[27].movCohete_z[1] = -47.416667f;
    KeyFrame[27].giroCohete_z[1] = 70.000000f;
    KeyFrame[27].movCohete_y[2] = 106.860875f;
    KeyFrame[27].giroCohete_z[2] = -8.750000f;
    KeyFrame[28] = KeyFrame[27];
    KeyFrame[28].movCohete_x[1] = -46.733333f;
    KeyFrame[28].movCohete_y[1] = 49.060310f;
    KeyFrame[28].movCohete_z[1] = -46.333333f;
    KeyFrame[28].giroCohete_z[1] = 80.000000f;
    KeyFrame[28].movCohete_y[2] = 109.048375f;
    KeyFrame[28].giroCohete_z[2] = -10.000000f;
    KeyFrame[29] = KeyFrame[28];
    KeyFrame[29].movCohete_x[1] = -44.900000f;
    KeyFrame[29].movCohete_y[1] = 39.090658f;
    KeyFrame[29].movCohete_z[1] = -45.250000f;
    KeyFrame[29].giroCohete_z[1] = 90.000000f;
    KeyFrame[29].movCohete_y[2] = 111.235875f;
    KeyFrame[29].giroCohete_z[2] = -11.250000f;
    KeyFrame[30] = KeyFrame[29];
    KeyFrame[30].movCohete_x[1] = -43.066667f;
    KeyFrame[30].movCohete_y[1] = 27.850068f;
    KeyFrame[30].movCohete_z[1] = -44.166667f;
    KeyFrame[30].movCohete_y[2] = 113.423375f;
    KeyFrame[30].giroCohete_z[2] = -12.500000f;
    KeyFrame[31] = KeyFrame[30];
    KeyFrame[31].movCohete_x[1] = -41.233333f;
    KeyFrame[31].movCohete_y[1] = 15.338538f;
    KeyFrame[31].movCohete_z[1] = -43.083333f;
    KeyFrame[31].movCohete_y[2] = 115.610875f;
    KeyFrame[31].giroCohete_z[2] = -13.750000f;
    KeyFrame[32] = KeyFrame[31];
    KeyFrame[32].movCohete_x[1] = -39.400000f;
    KeyFrame[32].movCohete_y[1] = 1.556068f;
    KeyFrame[32].movCohete_z[1] = -42.000000f;
    KeyFrame[32].movCohete_y[2] = 117.798375f;
    KeyFrame[32].giroCohete_z[2] = -15.000000f;
    KeyFrame[33] = KeyFrame[32];
    KeyFrame[33].movCohete_y[2] = 119.985875f;
    KeyFrame[33].giroCohete_z[2] = -16.250000f;
    KeyFrame[34] = KeyFrame[33];
    KeyFrame[34].movCohete_y[2] = 122.173375f;
    KeyFrame[34].giroCohete_z[2] = -17.500000f;
    KeyFrame[35] = KeyFrame[34];
    KeyFrame[35].movCohete_y[2] = 124.360875f;
    KeyFrame[35].giroCohete_z[2] = -18.750000f;
    KeyFrame[36] = KeyFrame[35];
    KeyFrame[36].movCohete_y[2] = 126.548375f;
    KeyFrame[36].giroCohete_z[2] = -20.000000f;
    KeyFrame[37] = KeyFrame[36];
    KeyFrame[37].movCohete_x[2] = -53.635429f;
    KeyFrame[37].movCohete_z[2] = -56.022225f;
    KeyFrame[37].giroCohete_y[2] = 15.000000f;
    KeyFrame[38] = KeyFrame[37];
    KeyFrame[38].movCohete_x[2] = -46.400000f;
    KeyFrame[38].movCohete_z[2] = -59.019238f;
    KeyFrame[38].giroCohete_y[2] = 30.000000f;
    KeyFrame[39] = KeyFrame[38];
    KeyFrame[39].movCohete_x[2] = -40.186797f;
    KeyFrame[39].movCohete_z[2] = -63.786797f;
    KeyFrame[39].giroCohete_y[2] = 45.000000f;
    KeyFrame[40] = KeyFrame[39];
    KeyFrame[40].movCohete_x[2] = -35.419238f;
    KeyFrame[40].movCohete_z[2] = -70.000000f;
    KeyFrame[40].giroCohete_y[2] = 60.000000f;
    KeyFrame[41] = KeyFrame[40];
    KeyFrame[41].movCohete_x[2] = -32.422225f;
    KeyFrame[41].movCohete_z[2] = -77.235429f;
    KeyFrame[41].giroCohete_y[2] = 75.000000f;
    KeyFrame[42] = KeyFrame[41];
    KeyFrame[42].movCohete_x[2] = -31.400000f;
    KeyFrame[42].movCohete_z[2] = -85.000000f;
    KeyFrame[42].giroCohete_y[2] = 90.000000f;
    KeyFrame[43] = KeyFrame[42];
    KeyFrame[43].movCohete_x[2] = -32.422225f;
    KeyFrame[43].movCohete_z[2] = -92.764571f;
    KeyFrame[43].giroCohete_y[2] = 105.000000f;
    KeyFrame[44] = KeyFrame[43];
    KeyFrame[44].movCohete_x[2] = -35.419238f;
    KeyFrame[44].movCohete_z[2] = -100.000000f;
    KeyFrame[44].giroCohete_y[2] = 120.000000f;
    KeyFrame[45] = KeyFrame[44];
    KeyFrame[45].movCohete_x[2] = -40.186797f;
    KeyFrame[45].movCohete_z[2] = -106.213203f;
    KeyFrame[45].giroCohete_y[2] = 135.000000f;
    KeyFrame[46] = KeyFrame[45];
    KeyFrame[46].movCohete_x[2] = -46.400000f;
    KeyFrame[46].movCohete_z[2] = -110.980762f;
    KeyFrame[46].giroCohete_y[2] = 150.000000f;
    KeyFrame[47] = KeyFrame[46];
    KeyFrame[47].movCohete_x[2] = -53.635429f;
    KeyFrame[47].movCohete_z[2] = -113.977775f;
    KeyFrame[47].giroCohete_y[2] = 165.000000f;
    KeyFrame[48] = KeyFrame[47];
    KeyFrame[48].movCohete_x[2] = -61.400000f;
    KeyFrame[48].movCohete_z[2] = -115.000000f;
    KeyFrame[48].giroCohete_y[2] = 180.000000f;
    KeyFrame[49] = KeyFrame[48];
    KeyFrame[49].movCohete_x[2] = -69.164571f;
    KeyFrame[49].movCohete_z[2] = -113.977775f;
    KeyFrame[49].giroCohete_y[2] = 195.000000f;
    KeyFrame[50] = KeyFrame[49];
    KeyFrame[50].movCohete_x[2] = -76.400000f;
    KeyFrame[50].movCohete_z[2] = -110.980762f;
    KeyFrame[50].giroCohete_y[2] = 210.000000f;
    KeyFrame[51] = KeyFrame[50];
    KeyFrame[51].movCohete_x[2] = -82.613203f;
    KeyFrame[51].movCohete_z[2] = -106.213203f;
    KeyFrame[51].giroCohete_y[2] = 225.000000f;
    KeyFrame[52] = KeyFrame[51];
    KeyFrame[52].movCohete_x[2] = -87.380762f;
    KeyFrame[52].movCohete_z[2] = -100.000000f;
    KeyFrame[52].giroCohete_y[2] = 240.000000f;
    KeyFrame[53] = KeyFrame[52];
    KeyFrame[53].movCohete_x[2] = -90.377775f;
    KeyFrame[53].movCohete_z[2] = -92.764571f;
    KeyFrame[53].giroCohete_y[2] = 255.000000f;
    KeyFrame[54] = KeyFrame[53];
    KeyFrame[54].movCohete_x[2] = -91.400000f;
    KeyFrame[54].movCohete_z[2] = -85.000000f;
    KeyFrame[54].giroCohete_y[2] = 270.000000f;
    KeyFrame[55] = KeyFrame[54];
    KeyFrame[55].movCohete_x[2] = -90.377775f;
    KeyFrame[55].movCohete_z[2] = -77.235429f;
    KeyFrame[55].giroCohete_y[2] = 285.000000f;
    KeyFrame[56] = KeyFrame[55];
    KeyFrame[56].movCohete_x[2] = -87.380762f;
    KeyFrame[56].movCohete_z[2] = -70.000000f;
    KeyFrame[56].giroCohete_y[2] = 300.000000f;
    KeyFrame[57] = KeyFrame[56];
    KeyFrame[57].movCohete_x[2] = -82.613203f;
    KeyFrame[57].movCohete_z[2] = -63.786797f;
    KeyFrame[57].giroCohete_y[2] = 315.000000f;
    KeyFrame[58] = KeyFrame[57];
    KeyFrame[58].movCohete_x[2] = -76.400000f;
    KeyFrame[58].movCohete_z[2] = -59.019238f;
    KeyFrame[58].giroCohete_y[2] = 330.000000f;
    KeyFrame[59] = KeyFrame[58];
    KeyFrame[59].movCohete_x[2] = -69.164571f;
    KeyFrame[59].movCohete_z[2] = -56.022225f;
    KeyFrame[59].giroCohete_y[2] = 345.000000f;
    KeyFrame[60] = KeyFrame[59];
    KeyFrame[60].movCohete_x[2] = -61.400000f;
    KeyFrame[60].movCohete_z[2] = -55.000000f;
    KeyFrame[60].giroCohete_y[2] = 360.000000f;
    // FIN KEYFRAMES COHETE
	if (!ValleHelado::iniciar()) return 2;
	if (grabarCohete) {
		std::filesystem::create_directories("../EvidenciasCohete/frames");
		CoheteKeyframes::guardar("../EvidenciasCohete/cohete_keyframes.txt");
		CoheteKeyframes::iniciarReproduccion();
	}

	camera = Camera(glm::vec3(0.0f, 0.0f, 375.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoPlaneta2 = Texture("Textures/pisoPlaneta2.jpg");
	pisoPlaneta2.LoadTextureA();

	// ----- Carga de modelos -----
	// Rocky 
	rocky = Model();
	rocky.LoadModel("Models/rockyPersonaje.obj");

	brazoRocky = Model();
	brazoRocky.LoadModel("Models/rockyBrazo.obj");

	// Piso Planeta 2
	
	rocas1 = Model();
	rocas1.LoadModel("Models/rocas1.obj");

	// Modelos del espacio
	sol = Model();
	sol.LoadModel("Models/sol.obj");

	luna = Model();
	luna.LoadModel("Models/luna.obj");

	anilloAst = Model();
	anilloAst.LoadModel("Models/anilloAst.obj");

	planeta1 = Model();
	planeta1.LoadModel("Models/planeta1.obj");

	planeta2 = Model();
	planeta2.LoadModel("Models/planeta2.obj");

	cuerpoSat = Model();
	cuerpoSat.LoadModel("Models/cuerpoSat.obj");

	antenaSat = Model();
	antenaSat.LoadModel("Models/antena.obj");

	panelIzqSat = Model();
	panelIzqSat.LoadModel("Models/panelIzq.obj");

	panelDerSat = Model();
	panelDerSat.LoadModel("Models/panelDer.obj");

	asteroides = Model();
	asteroides.LoadModel("Models/asteroides.obj");

	// ----- ConfiguraciÃ³n Skybox Escenario 1 -----
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/espacio_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_up.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/espacio_ft.tga");

	skybox1 = Skybox(skyboxFaces);
	
	// ----- ConfiguraciÃ³n Skybox Escenario 2 (Planeta 1)-----
	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/planeta1_rt.tga"); // Cambia estas rutas por tus texturas
	skyboxFaces2.push_back("Textures/Skybox/planeta1_lf.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_dn.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_up.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_bk.tga");
	skyboxFaces2.push_back("Textures/Skybox/planeta1_ft.tga");
	skybox2 = Skybox(skyboxFaces2);
	// ----- ConfiguraciÃ³n Skybox Escenario 3 (Planeta 2) -----
	std::vector<std::string> skyboxFaces3;
	skyboxFaces3.push_back("Textures/Skybox/erid_right.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_left.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_bottom.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_top.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_back.jpg");
	skyboxFaces3.push_back("Textures/Skybox/erid_front.jpg");
	skybox3 = Skybox(skyboxFaces3);
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//DeclaraciÃ³n de primer luz puntual (Modelo del Sol)
	pointLights[0] = PointLight(1.0f, 0.75f, 0.0f,
		5.0f, 5.0f,				// radiacion y color (0.3f y 0.6f)
		0.0f, 75.0f, -150.0f,
		0.3f, 0.0005f, 0.0003f); 
	pointLightCount++;

	//DeclaraciÃ³n de primer luz puntual
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

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	// Matrices y posiciones de las luces para la jerarquia
	glm::mat4 roverMatrix(1.0f);
	glm::mat4 avionMatrix(1.0f);

	glm::vec3 roverPos;
	glm::vec3 roverDir;

	glm::vec3 avionFrentPos;
	glm::vec3 avionFrentDir;

	glm::vec3 avionAbajoPos;
	glm::vec3 avionAbajoDir;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{	
		//luz direccional, sÃ³lo 1 y siempre debe de existir
		if (escenarioActual == 1)
		{
			// Espacio
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.4f, 0.4f,
				0.0f, -1.0f, 0.0f
			);
		}
		else if (escenarioActual == 2)
		{
			// Planeta 1
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.3f, 0.3f,
				1.0f, -1.0f, 0.0f
			);
		}
		else if (escenarioActual == 3)
		{
			// Planeta 2
			mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
				0.25f, 0.25f,
				1.0f, -1.0f, 0.0f
			);
		}

		GLfloat now = glfwGetTime();
		const double segundosValle = grabarCohete ? 1.0 / 24.0 : (verificar ? 1.0 / 60.0 : now - lastTime);
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), escenarioActual == 2 ? static_cast<float>(segundosValle) : deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		
		// DetecciÃ³n de teclas para cambiar de escenario y reubicar la cÃ¡mara
		if (mainWindow.getsKeys()[GLFW_KEY_1] && escenarioActual != 1) {
			escenarioActual = 1;
			camera = Camera(glm::vec3(0.0f, 0.0f, 375.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 2.5f, 0.5f);
			glfwSetWindowTitle(glfwGetCurrentContext(), "Proyecto final | 1 Espacio | 2 Valle helado | 3 Erid");
		}
		if (mainWindow.getsKeys()[GLFW_KEY_2] && escenarioActual != 2) {
			escenarioActual = 2;
			camera = Camera(glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);
		}
		if (mainWindow.getsKeys()[GLFW_KEY_3] && escenarioActual != 3) {
			escenarioActual = 3;
			camera = Camera(glm::vec3(0.0f, 0.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);
			glfwSetWindowTitle(glfwGetCurrentContext(), "Proyecto final | 1 Espacio | 2 Valle helado | 3 Erid");
		}
		if (verificarValle || grabarCohete) escenarioActual = 2;
		else if (verificarTodo) {
			if (framePrueba == 1) escenarioActual = 2;
			else if (framePrueba == 61) {
				escenarioActual = 3;
				camera = Camera(glm::vec3(0.0f, 8.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -5.0f, 0.5f, 0.5f);
			}
		}

		// Escenario 2: Valle helado, Crepper NPC y modelos de Planeta 1
		if (escenarioActual == 2) {
			if (!grabarCohete) CoheteKeyframes::inputKeyframes(mainWindow.getsKeys());
			if (mainWindow.getsKeys()[GLFW_KEY_C]) CoheteKeyframes::encuadrar(camera);
            // La captura toma 24 muestras/s de la misma secuencia de pasos a 60 cuadros/s.
            int avances = grabarCohete ? (framePrueba == 0 ? 0 :
                framePrueba*60/24 - (framePrueba-1)*60/24) : 1;
            for (int paso=0; paso<avances; ++paso) CoheteKeyframes::animate();
			auto proyeccionValle = glm::perspective(glm::radians(55.0f),
				(float)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1200.0f);
			ValleHelado::dibujar(camera, proyeccionValle, segundosValle);
			if (grabarCohete) {
				// Fotogramas del framebuffer real de OpenGL, a 24 FPS deterministas.
				int w=mainWindow.getBufferWidth(), h=mainWindow.getBufferHeight();
				std::vector<unsigned char> pixeles(static_cast<size_t>(w)*h*3);
				glPixelStorei(GL_PACK_ALIGNMENT,1); glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE,pixeles.data());
				char ruta[160]; snprintf(ruta,sizeof(ruta),"../EvidenciasCohete/frames/cohete_%04d.tga",framePrueba);
				std::ofstream foto(ruta,std::ios::binary);
				unsigned char cabecera[18]={0,0,2}; cabecera[12]=w&255; cabecera[13]=w>>8;
				cabecera[14]=h&255; cabecera[15]=h>>8; cabecera[16]=24;
				foto.write(reinterpret_cast<char*>(cabecera),18);
				foto.write(reinterpret_cast<char*>(pixeles.data()),pixeles.size());
				if (!foto || glGetError()!=GL_NO_ERROR) { printf("COHETE: fallo de captura\n"); falloGL=true; break; }
				if (++framePrueba>=24*48) break;
			}
			if (verificarValle) {
				if (framePrueba == 0 || framePrueba == 120 || framePrueba == 240) ValleHelado::captura(framePrueba);
				GLenum error = glGetError();
				if (error != GL_NO_ERROR) { printf("GL ERROR: %u\n", error); falloGL = true; }
				if (++framePrueba > 240) break;
			}
			else if (verificarTodo) {
				if (framePrueba == 30) ValleHelado::captura("x64/Practica9/escena2.ppm");
				if (++framePrueba > 62) break;
			}
			mainWindow.swapBuffers();
			continue;
		}
		ValleHelado::estabaActivo = false;
		
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// LÃ³gica de Renderizado por Escenario
		if (escenarioActual == 1) {
			skybox1.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else if (escenarioActual == 2) {
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else if (escenarioActual == 3) {
			skybox3.DrawSkybox(camera.calculateViewMatrix(), projection);
		}

		// ConfiguraciÃ³n de los Shaders de IluminaciÃ³n 
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaciÃ³n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cÃ¡mara de tipo flash
		//sirve para que en tiempo de ejecuciÃ³n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		
		if (escenarioActual == 1)
		{
			shaderList[0].SetDirectionalLight(&mainLight);
			shaderList[0].SetPointLights(pointLights, 1); // sÃ³lo el Sol
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
			shaderList[0].SetSpotLights(spotLights, 1); // sÃ³lo la linterna
		}

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0); // para la jerarquÃ­a del satelite
		glm::mat4 modelaux3(1.0); // para la jerarquÃ­a del satelite
		glm::mat4 modelaux4(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		// Dibujado de objetos segÃºn el escenario
		if (escenarioActual == 1) {

			// AnimaciÃ³n bÃ¡sica de los planetas
			rotaPlanetas += rotaPlanetasOffset * deltaTime;
			if (rotaPlanetas >= 360.0f * toRadians) {
				rotaPlanetas = 0.0f;
			}

			// AnimaciÃ³n bÃ¡sica de los anillos de asteroides
			rotaAnillo += rotaAnilloOffset * deltaTime;

			// AnimaciÃ³n de la luna orbitando alrededor del planeta 1
			rotaLuna += rotaLunaOffset * deltaTime;

			if (rotaLuna >= 360.0f * toRadians) {
				rotaLuna = 0.0f;
			}

			// AnimaciÃ³n del satÃ©lite
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

			// SatÃ©lite
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

			// CinturÃ³n de asteroides 1
			model = glm::mat4(1.0);
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 175.0f));
			model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rotaAnillo, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(1.55f, 1.55f, 1.55f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			anilloAst.RenderModel();

			// CinturÃ³n de asteroides 2
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
			// Coloca aquÃ­ los modelos, texturas y posiciones para tu Escenario 2

		}
		else if (escenarioActual == 3) {

			// AnimaciÃ³n de Rocky saludando
			if (saludo)
			{
				rotBrazoRocky += rotBrazoRockyOffset;

				if (rotBrazoRocky >= 30.0f)
					saludo = false;
			}
			else
			{
				rotBrazoRocky -= rotBrazoRockyOffset;

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


	
		/*
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);
		*/

		glUseProgram(0);

		if (verificarTodo) {
			if (framePrueba == 0) ValleHelado::captura("x64/Practica9/escena1.ppm");
			if (framePrueba == 61) ValleHelado::captura("x64/Practica9/escena3.ppm");
			if (++framePrueba > 62) break;
		}

		mainWindow.swapBuffers();
	}

	return falloGL ? 3 : 0;
}
