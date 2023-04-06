#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLWindow
{
private:
	// ����
	GLFWwindow* window;

	// glfw:ֻҪ���ڴ�С�ı�(�ɲ���ϵͳ���û�������С)������ص������ͻ�ִ��
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
public:
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;

	// ��������ȡ����������
	GLWindow(int SCR_WIDTH, int SCR_HEIGHT);

	void use();

	GLFWwindow* getWindow();
};