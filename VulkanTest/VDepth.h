#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "vk_mem_alloc.h"

class VDepth : VObject
{
public:
	VDepth(VkDevice device, VkPhysicalDevice physicalDevice, VmaAllocator allocator, VkExtent2D* swapChainExtent);
	void cleanUp();

	

	VkFormat getDepthFormat() {
		return depthFormat;
	}

	VkImageView* getDepthImageViewPtr() {
		return &depthImageView;
	}

	void recreate();

private:
	VkDevice device;
	VkPhysicalDevice physicalDevice;

	VkImage depthImage;
	VkImageViewCreateInfo viewInfo{};
	VkImageView depthImageView;
	VkFormat depthFormat;


	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	bool hasStencilComponent(VkFormat format);

	VkExtent2D* swapChainExtent;

	VkImageCreateInfo imageInfo = {};


	VmaAllocationCreateInfo imageAllocCreateInfo = {};
	VmaAllocationInfo depthImageAllocInfo = {};
	VmaAllocation depthImageAlloc;

	VmaAllocator allocator;

};

