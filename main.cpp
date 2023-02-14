#include "application.hpp"
#include<cstdlib>
#include<iostream>
#include<stdexcept>

using namespace std;

int main()
{
	lve::FirstApplication app{};
	
	try 
	{
		app.run();
	}
	catch (const exception& e)
	{
		cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

//int main() {
//    glfwInit();
//
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
//
//    uint32_t extensionCount = 0;
//    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//
//    std::cout << extensionCount << " extensions supported\n";
//
//    glm::mat4 matrix;
//    glm::vec4 vec;
//    auto test = matrix * vec;
//
//    while (!glfwWindowShouldClose(window)) {
//        glfwPollEvents();
//    }
//
//    glfwDestroyWindow(window);
//
//    glfwTerminate();
//
//    return 0;
