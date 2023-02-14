#include "vulkanWindow.hpp"
#include <iostream>


namespace lve
{
	VWindow::VWindow(int w, int h, string name) : width{ w }, height{ h }, windowName{ name }
	{
		initWindow();
	}

	VWindow::~VWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool VWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void VWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//handling resizing elsewhere
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}
}

