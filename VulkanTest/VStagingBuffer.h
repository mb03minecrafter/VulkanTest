#pragma once
#include "VBuffer.h"

class VStagingBuffer : VBuffer
{
public:
	VStagingBuffer(VmaAllocator allocator, VkDeviceSize size);
	void cleanUp();

	operator VkBuffer() const {
		return buffer;
	}
	

	void copyMemoryIntoBuffer(const void* srcPtr, VkDeviceSize offset, VkDeviceSize size);

private:

};

