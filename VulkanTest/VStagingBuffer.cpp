#include "VStagingBuffer.h"

VStagingBuffer::VStagingBuffer(VmaAllocator allocator, VkDeviceSize size) :
	VBuffer(allocator, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
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


