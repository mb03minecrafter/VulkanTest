#pragma once
#include <vulkan/vulkan.h>
#include "VObject.h"
class VDebugManager : VObject
{
public:
	VDebugManager(bool enableValidationlayers, VkInstance instance);

	void cleanUp();

private:
	VkDebugUtilsMessengerEXT debugMessenger;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	void populateDebugMessengerCreateInfo();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	VkInstance instance;

};

