#include "Window.h"
#include <iostream>
#include "Renderer.h"
static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

Window::Window(int width, int height, char* title, Renderer* renderer)
{

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwSetWindowUserPointer(window, renderer);

	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::cleanUp()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::windowShouldClose()
{
	return glfwWindowShouldClose(window);
}
