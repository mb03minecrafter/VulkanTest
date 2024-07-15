#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include "VPhysicalDevice.h"

class VCommandPool : VObject
{
public:
	VCommandPool(VkDevice device, QueueFamilyIndices indices);
	void cleanUp();

	operator VkCommandPool() const {
		return commandPool;
	}

private:
	VkDevice device;

	VkCommandPoolCreateInfo poolInfo{};
	VkCommandPool commandPool;



};

