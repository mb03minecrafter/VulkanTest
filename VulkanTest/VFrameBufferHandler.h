#pragma once
#include "VObject.h"
#include <vector>
#include <vulkan/vulkan.h>
#include <iostream>
#include "VImage.h"

class VFrameBufferHandler : VObject
{
public:
	VFrameBufferHandler(VkDevice device, VkRenderPass renderPass, VkRenderPass offscreenRenderPass, std::vector<VkImageView>* swapChainImageViews, VkExtent2D* swapChainExtent, VkImageView* depthImageView);

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





	void createFrameBuffers(VkRenderPass renderPass, std::vector<VkImageView>* swapChainImageViews, VkExtent2D swapChainExtent, VkImageView depthImageView);


	//deferred rendering offscreen imageviews
	std::vector<VkFramebuffer> offscreenFrameBuffers;

	//std::vector<VImage> positionBufferImageView;
	//std::vector<VImage> colorBufferImageView;
	//std::vector<VImage> normalBufferImageView;

	void createOffscreenImageViews(VkImage image, VkExtent2D extent);

	void createOffscreenFrameBuffer(VkRenderPass offScreenRenderPass, VkExtent2D swapChainExtent, VkImageView depthImageView);

};

