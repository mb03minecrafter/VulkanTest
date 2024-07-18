#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "Vertex.h"
#include "vk_mem_alloc.h"
class VBuffer : VObject
{
public:

	VBuffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memUsage, VmaAllocationCreateFlags flags);
	virtual void cleanUp();

	operator VkBuffer() const {
		return buffer;
	}

	void* getMappedData() {
		return allocInfo.pMappedData;
	}

private:

	VkBufferCreateInfo bufferInfo{};

	VmaAllocationInfo allocInfo;
	
	VmaAllocationCreateInfo allocCreateInfo;
	
protected:
	VkBuffer buffer;
	VmaAllocator allocator;
	VmaAllocation allocation;


};

