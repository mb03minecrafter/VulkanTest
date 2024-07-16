#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "Vertex.h"
#include "vk_mem_alloc.h"
class VBuffer : VObject
{
public:

	VBuffer(VmaAllocator allocator, VkDeviceSize size, int usage);
	virtual void cleanUp();

	operator VkBuffer() const {
		return buffer;
	}

private:

	VkBufferCreateInfo bufferInfo{};


	VmaAllocationCreateInfo allocInfo;
protected:
	VkBuffer buffer;
	VmaAllocator allocator;
	VmaAllocation allocation;


};

