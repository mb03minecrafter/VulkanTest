#include "VBuffer.h"
#include <stdexcept>
#include <iostream>

VBuffer::VBuffer(VmaAllocator allocator, VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memUsage, VmaAllocationCreateFlags flags) 
	: allocator(allocator)
{
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


	allocCreateInfo.usage = memUsage;
	allocCreateInfo.flags = flags;


	vmaCreateBuffer(allocator, &bufferInfo, &allocCreateInfo, &buffer, &allocation, &allocInfo);
	

}

void VBuffer::cleanUp()
{
	vmaDestroyBuffer(allocator, buffer, allocation);
}


