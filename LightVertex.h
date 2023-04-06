#pragma once
class LightVertex
{
private:
	static float vertices[];

	static unsigned int indices[];
public:
	unsigned int VAO, VBO, EBO;

	LightVertex();

	~LightVertex();
};

