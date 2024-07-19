#pragma once
#include "VCommandPool.h"
#include "VDevice.h"

class VMemTransferCommandPool : VCommandPool
{
public:
	VMemTransferCommandPool(VkDevice device, QueueFamilyIndices indices, VkQueue queue);
	void cleanUp();
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void copyBufferWithRegion(VkBuffer srcBuffer, VkBuffer dstBuffer, VkBufferCopy copyRegion);


	void copyVertexIndexBuffer(VkBuffer stagingBuffer, VkBuffer vertexBuffer, VkBuffer indexBuffer, VkBufferCopy vertexCopy, VkBufferCopy indexCopy);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);


private:
	VkQueue queue;

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

};

