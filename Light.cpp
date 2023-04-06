#include "Light.h"

Light::Light(int x, int y, int z)
{
	lightPos = glm::vec3(x, y, z);
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
}
