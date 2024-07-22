#include "VTextureImage.h"
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VTextureImage::VTextureImage(VkDevice device, std::string path, VmaAllocator allocator, VMemTransferCommandPool commandPool) :
    device(device),
    commandPool(commandPool),
    graphicsQueue(graphicsQueue)
{
    grabTexture(path, allocator);

    VkExtent2D imageExtent = {};
    imageExtent.width = texWidth;
    imageExtent.height = texHeight;

    textureImage = std::make_unique<VImage>(device, allocator); //imageExtent, VK_FORMAT_R8G8B8A8_SRGB);

    textureImage->createImage(imageExtent, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

    commandPool.transitionImageLayout(textureImage->getImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    commandPool.copyBufferToImage(*stagingBuffer, textureImage->getImage(), static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

    commandPool.transitionImageLayout(textureImage->getImage(), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    stagingBuffer->cleanUp();

    textureImage->createImageView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

    
}

void VTextureImage::cleanUp()
{
    
    textureImage->cleanUp();
}

void VTextureImage::grabTexture(std::string path, VmaAllocator allocator)
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




