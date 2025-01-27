#include "VFrameBufferHandler.h"
#include <stdexcept>
#include <array>

VFrameBufferHandler::VFrameBufferHandler(VkDevice device, VkRenderPass renderPass, VkRenderPass offscreenRenderPass, std::vector<VkImageView>* swapChainImageViews, VkExtent2D* swapChainExtent, VkImageView* depthImageView)
    : device(device),
    renderPass(renderPass),
    swapChainImageViews(swapChainImageViews),
    swapChainExtent(swapChainExtent),
    depthImageView(depthImageView)
{

    //positionBufferImageView.resize(swapChainImageViews->size());
    //colorBufferImageView.resize(swapChainImageViews->size());
    //normalBufferImageView.resize(swapChainImageViews->size());



    createFrameBuffers(renderPass, swapChainImageViews, *swapChainExtent, *depthImageView);
    createOffscreenFrameBuffer(offscreenRenderPass, *swapChainExtent, *depthImageView);
}

void VFrameBufferHandler::cleanUp()
{
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }

    for (auto framebuffer : offscreenFrameBuffers) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);

    }

}

void VFrameBufferHandler::recreate()
{
    createFrameBuffers(renderPass, swapChainImageViews, *swapChainExtent, *depthImageView);
}

void VFrameBufferHandler::createFrameBuffers(VkRenderPass renderPass, std::vector<VkImageView>* swapChainImageViews, VkExtent2D swapChainExtent, VkImageView depthImageView)
{
    swapChainFramebuffers.resize(swapChainImageViews->size());


    for (size_t i = 0; i < swapChainImageViews->size(); i++) {
        std::array<VkImageView, 2> attachments = {
            (*swapChainImageViews)[i],
            depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void VFrameBufferHandler::createOffscreenImageViews(VkImage image, VkExtent2D extent)
{

}

void VFrameBufferHandler::createOffscreenFrameBuffer(VkRenderPass offScreenRenderPass, VkExtent2D swapChainExtent, VkImageView depthImageView)
{
    /*offscreenFrameBuffers.resize(swapChainFramebuffers.size());


    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        std::array<VkImageView, 4> attachments = {
            positionBufferImageView[i].getImageView(),
            colorBufferImageView[i].getImageView(),
            normalBufferImageView[i].getImageView(),
            depthImageView
        };
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = offScreenRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

    }*/
}
