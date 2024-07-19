#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>

class VDescriptorPool : VObject
{
public:
	VDescriptorPool(VkDevice device, int MAX_FRAMES_IN_FLIGHT);

	void cleanUp();

	

	VkDescriptorPool getDescriptorPool() {
		return descriptorPool;
	}

private:
	VkDevice device;

	VkDescriptorPoolCreateInfo poolInfo{};
	VkDescriptorPool descriptorPool;

};

