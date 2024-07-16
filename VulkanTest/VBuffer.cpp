#include "VBuffer.h"
#include <stdexcept>
#include <iostream>

VBuffer::VBuffer(VmaAllocator allocator, VkDeviceSize size, int usage) 
	: allocator(allocator)
{
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


	allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
	allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;


	vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);
	

}

void VBuffer::cleanUp()
{
	vmaDestroyBuffer(allocator, buffer, allocation);
}


