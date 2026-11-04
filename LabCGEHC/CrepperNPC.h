#pragma once
#include <cmath>
#include <cstring>
#include <algorithm>
#include "CrepperDatos.h"

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
