#pragma once
#include "VObject.h"
#include <vector>
#include <vulkan/vulkan.h>
#include <iostream>

class VFrameBufferHandler : VObject
{
public:
	VFrameBufferHandler(VkDevice device, VkRenderPass renderPass, std::vector<VkImageView>* swapChainImageViews, VkExtent2D* swapChainExtent, VkImageView* depthImageView);

	void cleanUp();

	VkFramebuffer getFrameBuffer(int index) {

		
		return swapChainFramebuffers[index];
	}
	void recreate();

private:
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkDevice device;
	VkRenderPass renderPass;
	std::vector<VkImageView>* swapChainImageViews;
	VkExtent2D* swapChainExtent;

	VkImageView* depthImageView;


	void createFrameBuffers(VkDevice device, VkRenderPass renderPass, std::vector<VkImageView>* swapChainImageViews, VkExtent2D swapChainExtent, VkImageView depthImageView);
};

