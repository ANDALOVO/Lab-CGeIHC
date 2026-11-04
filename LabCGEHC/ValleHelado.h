#pragma once
#include <vector>
#include <fstream>
#include <algorithm>
#include <initializer_list>
#include "Texture.h"
#include "CrepperNPC.h"

// Escenario 2: Valle helado con Crepper NPC y modelos de Planeta 1.
namespace ValleHelado {
inline Model piezas[CrepperDatos::cantidad];
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
    for(int i=1;i<CrepperDatos::cantidad;++i) {
        piezas[i].LoadModel(CrepperDatos::nodos[i].archivo);
        if(!piezas[i].cargado()) return false;
    }
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
    camera=Camera({50,27,65},{0,1,0},-125,-15,24,.25f);
    glfwSetWindowTitle(glfwGetCurrentContext(),"Valle helado - Crepper NPC | 1 Espacio | 2 Valle | 3 Erid");
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

    // Crepper NPC jerarquizado
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
