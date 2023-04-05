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
	// ��������ȡ����������
	GLWindow(const int SCR_WIDTH, const int SCR_HEIGHT);

	void use();

	GLFWwindow* getWindow();
};