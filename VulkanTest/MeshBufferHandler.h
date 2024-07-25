#pragma once
#include "VStagingBuffer.h"
#include "VBuffer.h"
#include <memory>
#include "VMemTransferCommandPool.h"
class MeshBufferHandler
{
public:
	MeshBufferHandler(VmaAllocator allocator, std::vector<Vertex> vertices, std::vector< uint32_t> indices);

	void cleanUp();

	std::unique_ptr<VStagingBuffer> stagingBuffer;
	std::unique_ptr<VBuffer> vertexBuffer;
	std::unique_ptr<VBuffer> indexBuffer;

	void transferStagingBuffer(VMemTransferCommandPool commandPool);

private:

	VkDeviceSize vertexBufferSize; 
	VkDeviceSize indexBufferSize; 
	VkDeviceSize stagingBufferSize; 
	void clearStagingBuffer();


	std::vector<Vertex>& vertices;
	std::vector< uint32_t>& indices;
};

