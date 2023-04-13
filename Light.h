#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
	glm::vec3 lightPos;
	glm::mat4 model;
	glm::vec3 lightColor;

	Light(int x, int y, int z);
};

