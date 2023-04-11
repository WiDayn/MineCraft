#pragma once
#include "Camera.h"
#include "Creature.h"

class Player : public Creature
{
public:
	Camera camera;
	Player(int x, int y, int z, int HP);
};

