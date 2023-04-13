#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow
{
private:
	// 窗体
	GLFWwindow* window;

	// glfw:只要窗口大小改变(由操作系统或用户调整大小)，这个回调函数就会执行
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
public:
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;

	// 构造器读取并构建窗体
	GLWindow(int SCR_WIDTH, int SCR_HEIGHT);

	void use();

	GLFWwindow* getWindow();
};