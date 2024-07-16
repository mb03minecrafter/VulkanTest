#pragma once
#include "VStagingBuffer.h"
#include "VBuffer.h"
#include <memory>
#include "VMemTransferCommandPool.h"
class MeshBufferHandler
{
public:
	MeshBufferHandler(VmaAllocator allocator, VkDeviceSize size);

	void cleanUp();

	std::unique_ptr<VStagingBuffer> stagingBuffer;
	std::unique_ptr<VBuffer> vertexBuffer;

	void transferStagingBuffer(VMemTransferCommandPool commandPool);

private:

	VkDeviceSize size;

	void clearStagingBuffer();

};

