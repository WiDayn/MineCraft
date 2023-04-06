#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	glm::vec3 cameraPos, cameraFront, cameraUp;
	glm::mat4 view, projection;

	Camera();

	// ������Ļ��������view��projection����
	void update(int SCR_WIDTH, int SCR_HEIGHT);
};

