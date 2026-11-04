#include <iostream>
#include <GL/glew.h>    
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PlanetaryRaceTrack.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Asegúrate de que la ruta sea correcta según tu proyecto

const unsigned int WIN_W = 1280;
const unsigned int WIN_H = 720;

glm::vec3 camPos = glm::vec3(10.0f, 55.0f, 195.0f);
glm::vec3 camFwd = glm::normalize(glm::vec3(0.0f, -0.35f, -0.93f));
glm::vec3 camUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

float cYaw = -90.0f, cPitch = -20.0f;
float pX = WIN_W / 2.0f, pY = WIN_H / 2.0f;
bool isFirst = true;
float dT = 0.0f, prevT = 0.0f;

const char* vCode = R"(
#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNorm;

out vec2 uvData;
out vec3 normData;
out vec3 fragWorldPos;

uniform mat4 mMat;
uniform mat4 vMat;
uniform mat4 pMat;

void main() {
    fragWorldPos = vec3(mMat * vec4(inPos, 1.0));
    normData = mat3(transpose(inverse(mMat))) * inNorm;
    uvData = inUV;
    gl_Position = pMat * vMat * vec4(fragWorldPos, 1.0);
}
)";

const char* fCode = R"(
#version 330 core
out vec4 finalColor;

in vec2 uvData;
in vec3 normData;
in vec3 fragWorldPos;

void main() {
    vec3 lDir = normalize(vec3(0.5, 1.0, 0.4));
    float nDotL = max(dot(normalize(normData), lDir), 0.25);
    
    vec3 albedo;
    if (uvData.x >= 0.19 && uvData.x <= 0.81) {
        albedo = vec3(0.20, 0.22, 0.25);
        if (abs(uvData.x - 0.5) < 0.015 && fract(uvData.y * 0.5) < 0.5) {
            albedo = vec3(0.95, 0.80, 0.15); 
        }
    } else {
        float st = step(0.5, fract(uvData.y * 0.5));
        albedo = mix(vec3(0.85, 0.15, 0.15), vec3(0.92, 0.92, 0.92), st);
    }
    finalColor = vec4(albedo * nDotL, 1.0);
}
)";

void cbResize(GLFWwindow* w, int width, int height) { glViewport(0, 0, width, height); }

void cbMouse(GLFWwindow* w, double mx, double my) {
    float x = (float)mx, y = (float)my;
    if (isFirst) { pX = x; pY = y; isFirst = false; }
    float ox = (x - pX) * 0.1f, oy = (pY - y) * 0.1f;
    pX = x; pY = y;
    cYaw += ox; cPitch += oy;
    if (cPitch > 89.0f) cPitch = 89.0f;
    if (cPitch < -89.0f) cPitch = -89.0f;
    glm::vec3 f;
    f.x = cos(glm::radians(cYaw)) * cos(glm::radians(cPitch));
    f.y = sin(glm::radians(cPitch));
    f.z = sin(glm::radians(cYaw)) * cos(glm::radians(cPitch));
    camFwd = glm::normalize(f);
}

void checkInputs(GLFWwindow* w) {
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(w, true);
    float vel = ((glfwGetKey(w, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) ? 90.0f : 40.0f) * dT;

    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) camPos += vel * camFwd;
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) camPos -= vel * camFwd;
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) camPos -= glm::normalize(glm::cross(camFwd, camUpDir)) * vel;
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) camPos += glm::normalize(glm::cross(camFwd, camUpDir)) * vel;
    if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS) camPos += vel * camUpDir;
    if (glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camPos -= vel * camUpDir;

    if (glfwGetKey(w, GLFW_KEY_M) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(w, GLFW_KEY_N) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(WIN_W, WIN_H, "Pista de Carreras - GLEW", NULL, NULL);
    if (!win) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, cbResize);
    glfwSetCursorPosCallback(win, cbMouse);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Inicialización correcta de GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW\n";
        return -1;
    }
    glEnable(GL_DEPTH_TEST);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vCode, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fCode, NULL);
    glCompileShader(fs);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs); glAttachShader(prog, fs);
    glLinkProgram(prog);

    PlanetaryRaceTrack raceTrack;
    raceTrack.buildMesh();

    while (!glfwWindowShouldClose(win)) {
        float currT = (float)glfwGetTime();
        dT = currT - prevT; prevT = currT;

        checkInputs(win);

        glClearColor(0.06f, 0.07f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(prog);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)WIN_W / (float)WIN_H, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(camPos, camPos + camFwd, camUpDir);
        glm::mat4 model = glm::mat4(1.0f);

        glUniformMatrix4fv(glGetUniformLocation(prog, "pMat"), 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(glGetUniformLocation(prog, "vMat"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(prog, "mMat"), 1, GL_FALSE, glm::value_ptr(model));

        raceTrack.draw();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}