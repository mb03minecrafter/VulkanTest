#include "MeshBufferHandler.h"

MeshBufferHandler::MeshBufferHandler(VmaAllocator allocator, std::vector<Vertex> vertices, std::vector< uint16_t> indices) :
	vertices(vertices),
	indices(indices)
{


	vertexBufferSize = sizeof(vertices[0]) * vertices.size();
	indexBufferSize = sizeof(indices[0]) * indices.size();
	stagingBufferSize = vertexBufferSize + indexBufferSize;

	stagingBuffer = std::make_unique<VStagingBuffer>(allocator, stagingBufferSize);

	vertexBuffer = std::make_unique<VBuffer>(allocator, vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,0);
	indexBuffer = std::make_unique<VBuffer>(allocator, vertexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,0);


	stagingBuffer->copyMemoryIntoBuffer(vertices.data(), 0, vertexBufferSize);
	stagingBuffer->copyMemoryIntoBuffer(indices.data(), vertexBufferSize, indexBufferSize);

}

void MeshBufferHandler::cleanUp()
{
	vertexBuffer->cleanUp();
	indexBuffer->cleanUp();

}

void MeshBufferHandler::transferStagingBuffer(VMemTransferCommandPool commandPool)
{
	//commandPool.copyBuffer(*stagingBuffer, *vertexBuffer, vertexBufferSize);
	VkBufferCopy vertexCopy{};
	vertexCopy.dstOffset = 0;
	vertexCopy.srcOffset = 0;
	vertexCopy.size = vertexBufferSize;

	VkBufferCopy indexCopy{};
	indexCopy.dstOffset = 0;
	indexCopy.srcOffset = vertexBufferSize;
	indexCopy.size = indexBufferSize;

	commandPool.copyVertexIndexBuffer(*stagingBuffer, *vertexBuffer, *indexBuffer, vertexCopy, indexCopy);

	clearStagingBuffer();
}

void MeshBufferHandler::clearStagingBuffer()
{
	stagingBuffer->cleanUp();
	
}
