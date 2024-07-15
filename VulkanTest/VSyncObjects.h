#pragma once
#include <vulkan/vulkan.h>
#include "VObject.h"
#include <vector>

class VSyncObjects : VObject
{
public:
	VSyncObjects(VkDevice device, int MAX_FRAMES_IN_FLIGHT);
	void cleanUp();

	VkSemaphore getImageAvailableSemaphore(int index) {
		return imageAvailableSemaphores[index];
	}

	VkSemaphore getRenderFinishedSemaphore(int index) {
		return renderFinishedSemaphores[index];
	}
	
	VkFence* getInFlightFencePtr(int index) {
		return &(inFlightFences[index]);
	}

	VkFence getInFlightFence(int index) {
		return inFlightFences[index];
	}

private:
	int MAX_FRAMES_IN_FLIGHT;
	VkDevice device;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
};

