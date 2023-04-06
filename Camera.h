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

	// 输入屏幕长宽，更新view和projection矩阵
	void update(int SCR_WIDTH, int SCR_HEIGHT);
};

