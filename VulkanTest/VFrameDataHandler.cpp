#include "VFrameDataHandler.h"
#include <stdexcept>

VFrameDataHandler::VFrameDataHandler(VkDevice device, VkCommandPool commandPool, int MAX_FRAMES_IN_FLIGHT) :
	device(device),
	MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT)
{
	frameData.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;


	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;


	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &frameData[i].imageAvailableSemaphore) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &frameData[i].renderSemaphore) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &frameData[i].inFlightFence) != VK_SUCCESS || 
			vkAllocateCommandBuffers(device, &allocInfo, &frameData[i].commandBuffer)) {

			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}

		std::vector<DescriptorAllocatorGrowable::PoolSizeRatio> frameSizes = {
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4 },
		};

		frameData[i].frameDescriptors = DescriptorAllocatorGrowable{};
		frameData[i].frameDescriptors.init(device, 1000, frameSizes);


	}


	
	
	

}

void VFrameDataHandler::cleanUp()
{
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device, frameData[i].imageAvailableSemaphore, nullptr);
		vkDestroySemaphore(device, frameData[i].renderSemaphore, nullptr);
		vkDestroyFence(device, frameData[i].inFlightFence, nullptr);

		frameData[i].frameDescriptors.destroy_pools(device);

	}
}
