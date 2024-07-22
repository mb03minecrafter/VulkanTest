#include "VImage.h"
#include <stdexcept>

VImage::VImage(VkDevice device, VmaAllocator allocator, VkExtent2D extent, VkFormat format, VkImageUsageFlags usage, VkImageAspectFlags aspectFlag) :
	device(device),
	allocator(allocator),
    imageFormat(format),
    imageExtent(extent),
    imageUsage(usage),
    aspectFlag(aspectFlag)
{
    createImageAndView();

}

VImage::VImage(VkDevice device, VmaAllocator allocator) :
device(device),
allocator(allocator)
{
}

void VImage::createImageAndView()
{
    
    createImage(imageExtent, imageFormat, imageUsage);
    createImageView(imageFormat, aspectFlag);
}

void VImage::createImage(VkExtent2D extent, VkFormat format, VkImageUsageFlags usage)
{
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(extent.width);
    imageInfo.extent.height = static_cast<uint32_t>(extent.height);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.flags = 0; // Optional

    imageAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

    if (vmaCreateImage(allocator, &imageInfo, &imageAllocCreateInfo, &image, &imageAlloc, &imageAllocInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }
}

void VImage::createImageView(VkFormat format, VkImageAspectFlags aspectFlag)
{
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlag;//VK_IMAGE_ASPECT_DEPTH_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void VImage::cleanUp()
{
    vkDestroyImageView(device, imageView, nullptr);
    vmaDestroyImage(allocator, image, imageAlloc);
}
