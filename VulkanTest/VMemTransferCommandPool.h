#pragma once
#include "VCommandPool.h"
#include "VDevice.h"

class VMemTransferCommandPool : VCommandPool
{
public:
	VMemTransferCommandPool(VkDevice device, QueueFamilyIndices indices, VkQueue queue);
	void cleanUp();
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
private:
	VkQueue queue;
};

