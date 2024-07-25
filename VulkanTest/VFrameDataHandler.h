#pragma once
#include "VObject.h"
#include "FrameData.h"
#include <vector>

class VFrameDataHandler : VObject
{
public:
	VFrameDataHandler(VkDevice device, VkCommandPool commandPool, int MAX_FRAMES_IN_FLIGHT);
	void cleanUp();

	VkCommandBuffer getCommandBuffer(int index) {
		return frameData[index].commandBuffer;
	}

	VkCommandBuffer* getCommandBufferPtr(int index) {
		return &frameData[index].commandBuffer;
	}

	VkSemaphore getImageAvailableSemaphore(int index) {
		return frameData[index].imageAvailableSemaphore;
	}

	VkSemaphore getRenderSemaphore(int index) {
		return frameData[index].renderSemaphore;
	}

	VkFence getInFlightFence(int index) {
		return frameData[index].inFlightFence;
	}

	VkFence* getInFlightFencePtr(int index) {
		return &frameData[index].inFlightFence;
	}

	void clearPools(int index) {
		frameData[index].frameDescriptors.clear_pools(device);
	}

	void deletePools(int index) {
		frameData[index].frameDescriptors.destroy_pools(device);

	}

private:
	int MAX_FRAMES_IN_FLIGHT;
	VkDevice device;
	std::vector<FrameData> frameData;
};

