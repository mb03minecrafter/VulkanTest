#include "VDepth.h"
#include <stdexcept>
#include <iostream>

VDepth::VDepth(VkDevice device, VkPhysicalDevice physicalDevice, VmaAllocator allocator, VkExtent2D* swapChainExtent) :
    device(device),
    physicalDevice(physicalDevice),
    swapChainExtent(swapChainExtent)
    
{
    depthFormat = findDepthFormat();

    depthImage = std::make_unique<VImage>(device, allocator, *swapChainExtent, depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

}

VkFormat VDepth::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat VDepth::findDepthFormat()
{
    return findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

bool VDepth::hasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void VDepth::cleanUp()
{
    

    depthImage->cleanUp();

}

void VDepth::recreate()
{
    
    depthImage->setImageExent(*swapChainExtent);
    depthImage->createImageAndView();



}
