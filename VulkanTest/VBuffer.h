#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "Vertex.h"
class VBuffer : VObject
{
public:

	VBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlagBits usage, VkSharingMode sharingMode, std::vector<Vertex> vertices);
	void cleanUp();

	operator VkBuffer() const {
		return buffer;
	}

private:

	VkDevice device;
	VkPhysicalDevice physicalDevice;
	VkBufferCreateInfo bufferInfo{};

	VkBuffer buffer;
	VkDeviceMemory bufferMemory;

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
};

