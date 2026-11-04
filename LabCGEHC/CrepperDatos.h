#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <array>

namespace CrepperDatos {
struct Nodo { const char* nombre; const char* archivo; int padre; glm::vec3 traslacion; };
inline const Nodo nodos[] = {
    {"Crepper_Raiz", nullptr, -1, glm::vec3(0.000000000f, 0.000000000f, 0.000000000f)},
    {"cuerpo_inferior", "Models/CrepperJerarquizado/cuerpo_inferior.obj", 0, glm::vec3(0.000000000f, 0.670000000f, 0.050000000f)},
    {"cabeza", "Models/CrepperJerarquizado/cabeza.obj", 1, glm::vec3(0.000000000f, 0.240000000f, -1.440000000f)},
    {"borde_frontal", "Models/CrepperJerarquizado/borde_frontal.obj", 1, glm::vec3(0.000000000f, 0.000000000f, -1.975000000f)},
    {"placa_01", "Models/CrepperJerarquizado/placa_01.obj", 1, glm::vec3(0.000000000f, 0.000000000f, -1.660000000f)},
    {"placa_02", "Models/CrepperJerarquizado/placa_02.obj", 1, glm::vec3(0.000000000f, 0.000000000f, -1.010000000f)},
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
