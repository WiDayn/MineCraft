#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
Camera::Camera() {
	cameraPos = glm::vec3(0.0f, 3.0f, 0.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
}

void Camera::update(int SCR_WIDTH, int SCR_HEIGHT)
{
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	projection = glm::perspective(glm::radians(45.0f), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
}
