#include "VTextureImage.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VTextureImage::VTextureImage(VkDevice device, std::string path, VmaAllocator allocator, VMemTransferCommandPool commandPool) :
    device(device),
    allocator(allocator),
    commandPool(commandPool),
    graphicsQueue(graphicsQueue)
{
    grabTexture(path);

    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(texWidth);
    imageInfo.extent.height = static_cast<uint32_t>(texHeight);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional


    imageAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;


    if (vmaCreateImage(allocator, &imageInfo,&imageAllocCreateInfo, &textureImage, &textureImageAlloc, &textureImageAllocInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    commandPool.transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    commandPool.copyBufferToImage(*stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

    commandPool.transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    stagingBuffer->cleanUp();


    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = textureImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &viewInfo, nullptr, &textureImageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void VTextureImage::cleanUp()
{
    vkDestroyImageView(device, textureImageView, nullptr);
    vmaDestroyImage(allocator, textureImage, textureImageAlloc);
}

void VTextureImage::grabTexture(std::string path)
{
    stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    

    stagingBuffer = std::make_unique<VStagingBuffer>(allocator, imageSize);
    stagingBuffer->copyMemoryIntoBuffer(pixels, 0, imageSize);

    stbi_image_free(pixels);

}




