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

		bool shouldClose();

	private:

		void initWindow();

		const int width;
		const int height;

		string windowName;

		GLFWwindow* window;
	};

}
