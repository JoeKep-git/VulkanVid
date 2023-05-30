#include "VWindow.hpp"
#include <iostream>
#include <stdexcept>

/******************************************************************************

File        : VWindow.cpp

Date        : 2023

Author      : Joey Keputa

Description : This creates a vulkan window. Help from the vulkan tutorial videos
			  referenced in report. You may need to move the should close method 
			  by one line as sometimes an error occurs but it doesnt matter if
			  you move the method down one line and back to the same place. As
			  long as it is not above the other functions it will work.
******************************************************************************/

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
	
	void VWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//handling resizing elsewhere
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void VWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
	{
		if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw runtime_error("failed to create window surface");
		}
	}

	void VWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto vWindow = reinterpret_cast<VWindow*>(glfwGetWindowUserPointer(window));
		vWindow->framebufferResized = true;
		vWindow->width = width;
		vWindow->height = height;
	}


	//put this under create window surface
	bool VWindow::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	//forcefully closes the window
	void VWindow::closeWindow()
	{
		glfwDestroyWindow(window);
	}
}

