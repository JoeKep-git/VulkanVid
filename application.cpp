#include "application.hpp"

void lve::FirstApplication::run()
{
	while (!vWindow.shouldClose())
	{
		glfwPollEvents();
	}
}
