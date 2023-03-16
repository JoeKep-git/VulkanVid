#include "VWindow.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

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

	void VWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
	{
		if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw runtime_error("failed to create window surface");
		}
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

