#pragma once
#include <vulkan/vulkan.h>
#include "DescriptorAllocatorGrowable.h"

struct FrameData
{
	VkSemaphore renderSemaphore, imageAvailableSemaphore;
	VkFence inFlightFence;

	VkCommandBuffer commandBuffer;

	DescriptorAllocatorGrowable frameDescriptors;


};

