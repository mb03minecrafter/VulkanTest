#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>
class VImage : VObject
{
public:
	VImage(VkDevice device, VmaAllocator allocator, VkExtent2D extent, VkFormat format, VkImageUsageFlags usage, VkImageAspectFlags aspectFlag);
	VImage(VkDevice device, VmaAllocator allocator);

	void createImageAndView();

	void createImage(VkExtent2D extent, VkFormat format, VkImageUsageFlags usage);
	void createImageView(VkFormat format, VkImageAspectFlags aspectFlag);

	void cleanUp();

	VkImageView* getImageViewPtr() {
		return &imageView;
	}

	VkImageView getImageView() {
		return imageView;
	}

	VkImage getImage() {
		return image;
	}

	void setImage(VkImage image) {
		this->image = image;
	}

	void setImageExent(VkExtent2D extent) {
		imageExtent = extent;
	}

private:
	VkImage image;
	VkImageCreateInfo imageInfo = {};

	VkImageView imageView;
	VkImageViewCreateInfo viewInfo{};


	VmaAllocationCreateInfo imageAllocCreateInfo = {};
	VmaAllocationInfo imageAllocInfo = {};
	VmaAllocation imageAlloc;

	VkFormat imageFormat;
	VkExtent2D imageExtent;
	VkImageUsageFlags imageUsage;
	VkImageAspectFlags aspectFlag;

	VmaAllocator allocator;

	VkDevice device;

};

