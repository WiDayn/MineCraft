#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLWindow
{
private:
	// 窗体
	GLFWwindow* window;

	// glfw:只要窗口大小改变(由操作系统或用户调整大小)，这个回调函数就会执行
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
public:
	// 构造器读取并构建窗体
	GLWindow(const int SCR_WIDTH, const int SCR_HEIGHT);

	void use();

	GLFWwindow* getWindow();
};