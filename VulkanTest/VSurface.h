#pragma once
#include "VObject.h"


#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>





class VSurface : VObject
{
public:

	VSurface(VkInstance instance, GLFWwindow* window);

	void cleanUp();

	operator VkSurfaceKHR() const {
		return surface;
	}

private:
	VkSurfaceKHR surface;
	VkInstance instance;


};

