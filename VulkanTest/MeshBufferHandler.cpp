#include "MeshBufferHandler.h"

MeshBufferHandler::MeshBufferHandler(VmaAllocator allocator, VkDeviceSize size) : size(size)
{
	stagingBuffer = std::make_unique<VStagingBuffer>(allocator, size);

	vertexBuffer = std::make_unique<VBuffer>(allocator, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
	

}

void MeshBufferHandler::cleanUp()
{
	//stagingBuffer->cleanUp();
	vertexBuffer->cleanUp();


}

void MeshBufferHandler::transferStagingBuffer(VMemTransferCommandPool commandPool)
{
	commandPool.copyBuffer(*stagingBuffer, *vertexBuffer, size);
	clearStagingBuffer();
}

void MeshBufferHandler::clearStagingBuffer()
{
	stagingBuffer->cleanUp();
	
}
