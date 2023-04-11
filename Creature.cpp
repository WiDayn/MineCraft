#include "Creature.h"

Creature::Creature(float x, float y, float z, float HP)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->HP = HP;
}

Creature::Creature()
{
}

float Creature::getX()
{
	return x;
}

float Creature::getY()
{
	return y;
}

float Creature::getZ()
{
	return z;
}

void Creature::setPos(float x, float y, float z)
{
	this->x = x, this->y = y, this->z = z;
}

bool Creature::moveX(float x)
{
	this->x += x;
	return true;
}

bool Creature::moveY(float y)
{
	this->y += y;
	return true;
}

bool Creature::moveZ(float z)
{
	this->z += z;
	return true;
}

bool canMove() {
	return true;
}
