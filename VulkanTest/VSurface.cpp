#include "VSurface.h"
#include <stdexcept>
#include <iostream>

VSurface::VSurface(VkInstance instance, GLFWwindow* window) : instance(instance)
{
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = glfwGetWin32Window(window);
	createInfo.hinstance = GetModuleHandle(nullptr);

	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}



}

void VSurface::cleanUp()
{
	vkDestroySurfaceKHR(instance, surface, nullptr);
}
