#include "Player.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player(int x, int y, int z, int HP)
{
	Creature(x, y, z, HP);
	this->camera = Camera();

}
