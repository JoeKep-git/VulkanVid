#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <string>

using namespace std;


namespace lve
{
	class VWindow 
	{
	public:
		VWindow(int w, int h, string name);
		~VWindow();

		VWindow(const VWindow&) = delete;
		VWindow &operator=(const VWindow&) = delete;

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }

		bool wasWindowResized() { return framebufferResized; }

	    void resetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

		bool shouldClose();

	private:

		static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

		void initWindow();

		int width;
		int height;
		bool framebufferResized = false;

		string windowName;

		GLFWwindow* window;
	};

}
