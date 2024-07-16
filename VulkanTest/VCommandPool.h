#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include "VPhysicalDevice.h"

class VCommandPool : VObject
{
public:
	VCommandPool(VkDevice device, QueueFamilyIndices indices, VkCommandPoolCreateFlags flags);
	void cleanUp();

	operator VkCommandPool() const {
		return commandPool;
	}

private:

	VkCommandPoolCreateInfo poolInfo{};

protected:
	VkDevice device;
	VkCommandPool commandPool;


};

