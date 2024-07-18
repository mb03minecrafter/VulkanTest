#pragma once
#include <vulkan/vulkan.h>
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "vk_mem_alloc.h"
#include "VObject.h"

class VAllocator : VObject
{
public:
	VAllocator(VkInstance instance, VkDevice device, VkPhysicalDevice physicalDevice);
	VmaVulkanFunctions vulkanFunctions = {};
	VmaAllocatorCreateInfo allocatorCreateInfo = {};

	VmaAllocator getAllocator() {
		return allocator;
	}

	void cleanUp();
private:
	VmaAllocator allocator;

};

