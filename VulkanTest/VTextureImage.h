#pragma once
#include "VObject.h"



#include <vulkan/vulkan.h>

#include <string>

#include <memory>
#include "VStagingBuffer.h"
#include "VMemTransferCommandPool.h"
#include <memory>
#include "VImage.h"


class VTextureImage : VObject
{
public:
	VTextureImage(VkDevice device, std::string path, VmaAllocator allocator, VMemTransferCommandPool commandPool);

	void cleanUp();

	VkImageView getTextureImageView() {
		return textureImage->getImageView();
	}



private:
	std::unique_ptr<VStagingBuffer> stagingBuffer;

	void grabTexture(std::string path, VmaAllocator allocator);

	int texWidth, texHeight, texChannels;

	VkDevice device;
	VMemTransferCommandPool commandPool;
	VkQueue graphicsQueue;
	std::unique_ptr<VImage> textureImage;
};

