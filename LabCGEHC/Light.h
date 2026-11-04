#pragma once

#include <glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);

	// Heredado por PointLight y SpotLight; conserva intensidad y atenuacion.
	void SetColor(const glm::vec3& nuevoColor) { color = nuevoColor; }

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

