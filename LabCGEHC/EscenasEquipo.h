#pragma once
#include <memory>
#include <map>
#include "PlanetaryRaceTrack.h"
#include "CrepperDatos.h"
#include "EscenasEquipoBase.h"

namespace Equipo {
inline int escenario = 2; // 1 espacio, 2 valle helado, 3 Erid.
inline std::map<std::string,std::unique_ptr<Model>> modelos;
inline Model crepper[CrepperDatos::cantidad];
inline PlanetaryRaceTrack pista;
inline Shader shaderPista;
inline double reloj = 0;
inline float distanciaPista = 0;
inline std::vector<float> distancias;

inline glm::mat4 colocar(Model& m, glm::vec3 posicion, float altura, float giro=0) {
    const auto& a=m.minimo; const auto& b=m.maximo;
    float escala=altura/(std::max)(0.001f,b.y-a.y);
    auto matriz=glm::translate(glm::mat4(1),posicion);
    matriz=glm::rotate(matriz,giro,glm::vec3(0,1,0));
    matriz=glm::scale(matriz,glm::vec3(escala));
    return glm::translate(matriz,glm::vec3(-(a.x+b.x)/2,-a.y,-(a.z+b.z)/2));
}
inline void objeto(const char* nombre, const glm::mat4& matriz, Shader& shader) {
    glUniformMatrix4fv(shader.GetModelLocation(),1,GL_FALSE,glm::value_ptr(matriz));
    glUniform3f(shader.getColorLocation(),1,1,1);
    modelos.at(nombre)->RenderModel();
}
inline void objeto(const char* nombre, glm::vec3 p,float altura,Shader& shader,float giro=0) {
    objeto(nombre,colocar(*modelos.at(nombre),p,altura,giro),shader);
}
inline bool iniciar() {
    iniciarBase();
    for(const char* file : {"aurelion sol.glb","torre de lanzamiento.glb","carroexplorador.obj",
        "Podium.obj","UFO.obj","alien_1.obj","Barra.obj","Barra1.obj","Barra2.obj","Barra3.obj",
        "TorsoAvatar.obj","BIAvatar.obj","BDAvatar.obj","ColaAvatar.obj","Torus.obj","naveEspacial.obj"}) {
        auto m=std::make_unique<Model>();
        m->LoadModel(std::string("Models/Equipo/")+file);
        if(!m->cargado()) { printf("Falta modelo del equipo: %s\n",file); return false; }
        modelos[file]=std::move(m);
    }
    for(int i=1;i<CrepperDatos::cantidad;++i) {
        crepper[i].LoadModel(CrepperDatos::nodos[i].archivo);
        if(!crepper[i].cargado()) return false;
    }
    pista.buildMesh();
    shaderPista.CreateFromFiles("shaders/pista_equipo.vert","shaders/pista_equipo.frag");
    distancias.push_back(0);
    for(size_t i=1;i<=pista.pathData.size();++i)
        distancias.push_back(distancias.back()+glm::length(pista.pathData[i%pista.pathData.size()].pos-pista.pathData[i-1].pos));
    return sol.cargado() && rocky.cargado() && brazoRocky.cargado() && cuerpoSat.cargado()
        && antenaSat.cargado() && panelIzqSat.cargado() && panelDerSat.cargado();
}
inline void cambiar(int nuevo, Camera& cam) {
    if(nuevo==escenario) return;
    escenario=nuevo;
    if(nuevo==1) cam=Camera({0,170,650},{0,1,0},-90,-15,100,0.2f);
    if(nuevo==2) cam=Camera({40,30,52},{0,1,0},-127.5f,-24,15,0.5f);
    if(nuevo==3) cam=Camera({90,85,210},{0,1,0},-110,-20,40,0.2f);
    const char* titulo=nuevo==1?"Proyecto final | 1 Espacio | 2 Valle helado | 3 Erid":
        nuevo==2?"Proyecto final - Valle helado | 1 Espacio | 2 Valle | 3 Erid":
        "Proyecto final - Erid y pista | 1 Espacio | 2 Valle | 3 Erid";
    glfwSetWindowTitle(glfwGetCurrentContext(),titulo);
    printf("Escenario %d\n",nuevo);
}
inline void controles(bool* keys, Camera& cam, float dt) {
    for(int i=1;i<=3;++i) if(keys[GLFW_KEY_0+i]) cambiar(i,cam);
    reloj+=dt;
    distanciaPista=std::fmod(distanciaPista+24*dt,distancias.back());
}
inline void dibujarHelado(Shader& shader) {
    shader.UseShader();
    objeto("torre de lanzamiento.glb",{-65,-2,15},45,shader);
    objeto("aurelion sol.glb",{-65,65,15},22,shader,static_cast<float>(reloj*.12));
    objeto("carroexplorador.obj",{35,-2,-20},8,shader);
    CrepperDatos::Matrices rotaciones;
    for(auto& m:rotaciones) m=glm::mat4(1);
    auto mundo=glm::scale(glm::translate(glm::mat4(1),glm::vec3(15,-2,10)),glm::vec3(5));
    auto matrices=CrepperDatos::calcularMatrices(mundo,rotaciones);
    for(int i=1;i<CrepperDatos::cantidad;++i) {
        glUniformMatrix4fv(shader.GetModelLocation(),1,GL_FALSE,glm::value_ptr(matrices[i]));
        glUniform3f(shader.getColorLocation(),1,1,1);
        crepper[i].RenderModel();
    }
    // Las piezas del avatar comparten el sistema de coordenadas de su exportacion.
    auto avatar=colocar(*modelos.at("TorsoAvatar.obj"),{65,-2,10},10);
    for(const char* parte:{"TorsoAvatar.obj","BIAvatar.obj","BDAvatar.obj","ColaAvatar.obj"}) objeto(parte,avatar,shader);
}
inline void dibujar(float dt,Camera& cam,const glm::mat4& projection) {
    dibujarBase(escenario,dt,cam,projection);
    if(escenario!=3) return;
    auto& shader=shaderList[0];
    Material_opaco.UseMaterial(shader.GetSpecularIntensityLocation(),shader.GetShininessLocation());
    objeto("Podium.obj",{-28,-1,0},12,shader);
    objeto("alien_1.obj",{-28,11,0},8,shader);
    objeto("UFO.obj",{35,30+3*std::sin(static_cast<float>(reloj)),0},10,shader,static_cast<float>(reloj*.25));
    for(int i=0;i<12;++i) {
        auto p=pista.pathData[(i*pista.pathData.size())/12].pos+glm::vec3(0,0,-100);
        const char* barras[]={"Barra.obj","Barra1.obj","Barra2.obj","Barra3.obj"};
        p.x+=10;
        objeto(barras[i%4],p,4,shader);
    }
    auto upper=std::upper_bound(distancias.begin(),distancias.end(),distanciaPista);
    size_t i=(std::max)(size_t(1),static_cast<size_t>(upper-distancias.begin()))-1;
    size_t j=(i+1)%pista.pathData.size();
    float f=(distanciaPista-distancias[i])/(distancias[i+1]-distancias[i]);
    auto p=glm::mix(pista.pathData[i].pos,pista.pathData[j].pos,f)+glm::vec3(0,2,-100);
    auto dir=glm::normalize(glm::mix(pista.pathData[i].fwd,pista.pathData[j].fwd,f));
    objeto("naveEspacial.obj",p,3,shader,std::atan2(dir.x,dir.z));
    objeto("Torus.obj",{0,1,-100},12,shader);
    shaderPista.UseShader();
    GLint program=0; glGetIntegerv(GL_CURRENT_PROGRAM,&program);
    auto pistaM=glm::translate(glm::mat4(1),glm::vec3(0,0,-100));
    auto view=cam.calculateViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(program,"mMat"),1,GL_FALSE,glm::value_ptr(pistaM));
    glUniformMatrix4fv(glGetUniformLocation(program,"vMat"),1,GL_FALSE,glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(program,"pMat"),1,GL_FALSE,glm::value_ptr(projection));
    pista.draw();
    glUseProgram(0);
}
}
