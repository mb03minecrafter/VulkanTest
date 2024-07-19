#pragma once
#include "VObject.h"



#include <vulkan/vulkan.h>

#include <string>

#include <memory>
#include "VStagingBuffer.h"
#include "VMemTransferCommandPool.h"



class VTextureImage : VObject
{
public:
	VTextureImage(VkDevice device, std::string path, VmaAllocator allocator, VMemTransferCommandPool commandPool);

	void cleanUp();

	VkImageView getTextureImageView() {
		return textureImageView;
	}



private:
	std::unique_ptr<VStagingBuffer> stagingBuffer;

	void grabTexture(std::string path);

	VmaAllocator allocator;

	VkImage textureImage;
	VkImageCreateInfo imageInfo = {};
	int texWidth, texHeight, texChannels;

	VmaAllocationCreateInfo imageAllocCreateInfo = {};
	VmaAllocationInfo textureImageAllocInfo = {};
	VmaAllocation textureImageAlloc;


	VkImageView textureImageView;
	VkImageViewCreateInfo viewInfo{};

	

	VkDevice device;
	VMemTransferCommandPool commandPool;
	VkQueue graphicsQueue;
};

