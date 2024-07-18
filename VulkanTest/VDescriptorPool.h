#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>

class VDescriptorPool : VObject
{
public:
	VDescriptorPool(VkDevice device, int MAX_FRAMES_IN_FLIGHT);

	void cleanUp();

	operator VkDescriptorPool() const {
		return descriptorPool;
	}

private:
	VkDevice device;

	VkDescriptorPoolSize poolSize{};
	VkDescriptorPoolCreateInfo poolInfo{};
	VkDescriptorPool descriptorPool;

};

