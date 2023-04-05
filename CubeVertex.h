#pragma once
class CubeVertex
{
private:
    static float vertices[];

    static unsigned int indices[];

public:
    unsigned int VBO, VAO, EBO;

    CubeVertex();

    ~CubeVertex();
};

