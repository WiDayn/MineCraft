#pragma once
class Creature
{
protected:
	float x, y, z;
	float HP;
public:
	Creature(float x, float y, float z, float HP);

	Creature();

	float getX();

	float getY();

	float getZ();

	void setPos(float x, float y, float z);

	bool moveX(float x);

	bool moveY(float y);
	
	bool moveZ(float z);
};

