#pragma once
#include <vulkan/vulkan.h>
#include <optional>
#include <vector>



inline const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}

};
class VPhysicalDevice
{

	

public:
	VPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);


	


	QueueFamilyIndices pickedQueueFamilyIndices;

	operator VkPhysicalDevice() const {
		return physicalDevice;
	}

	



private:
	VkPhysicalDevice physicalDevice;
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	VkSurfaceKHR surface;

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool querySwapChainSupport(VkPhysicalDevice device);

};

