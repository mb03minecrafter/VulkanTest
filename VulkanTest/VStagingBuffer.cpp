#include "VStagingBuffer.h"

VStagingBuffer::VStagingBuffer(VmaAllocator allocator, VkDeviceSize size) :
	VBuffer(allocator, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_AUTO, VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT)
{



}

void VStagingBuffer::cleanUp()
{
	VBuffer::cleanUp();

}



void VStagingBuffer::copyMemoryIntoBuffer(const void* srcPtr, VkDeviceSize offset, VkDeviceSize size)
{
	vmaCopyMemoryToAllocation(allocator, srcPtr, allocation, offset, size);
}


