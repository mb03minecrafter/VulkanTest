#include "VFrameBufferHandler.h"
#include <stdexcept>

VFrameBufferHandler::VFrameBufferHandler(VkDevice device, VkRenderPass renderPass, std::vector<VkImageView>& swapChainImageViews, VkExtent2D& swapChainExtent) 
    : device(device),
    renderPass(renderPass),
    swapChainImageViews(swapChainImageViews),
    swapChainExtent(swapChainExtent)
{
    createFrameBuffers(device, renderPass, swapChainImageViews, swapChainExtent);
}

void VFrameBufferHandler::cleanUp()
{
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
}

void VFrameBufferHandler::recreate()
{
    createFrameBuffers(device, renderPass, swapChainImageViews, swapChainExtent);
}

void VFrameBufferHandler::createFrameBuffers(VkDevice device, VkRenderPass renderPass, std::vector<VkImageView>& swapChainImageViews, VkExtent2D swapChainExtent)
{
    swapChainFramebuffers.resize(swapChainImageViews.size());


    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}
