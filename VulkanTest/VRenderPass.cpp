#include "VRenderPass.h"
#include <stdexcept>
#include <array>

VRenderPass::VRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat) : device(device)
{
	
	fillColorAttachment(colorFormat);
	fillDepthAttachment(depthFormat);
	
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.pDepthStencilAttachment = &depthAttachmentRef;


	
	std::array<VkSubpassDependency, 2> subpassDependencies = SetSubpassDependencies();

	std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };



	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = subpassDependencies.data();

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}


	createOffScreenRenderPass(colorFormat, depthFormat);

}

void VRenderPass::cleanUp()
{
	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroyRenderPass(device, offScreenRenderPass, nullptr);
}

void VRenderPass::fillColorAttachment(VkFormat colorFormat)
{
	colorAttachment.format = colorFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

void VRenderPass::fillDepthAttachment(VkFormat depthFormat)
{
	depthAttachment.format = depthFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	depthAttachmentRef.attachment = 1;
	depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

}



std::array<VkSubpassDependency, 2> VRenderPass::SetSubpassDependencies()
{
	std::array<VkSubpassDependency, 2> dependencies = {};

	// VK_IMAGE_LAYOUT_UNDEFINED -> VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[0].dstSubpass = 0;
	dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[0].dependencyFlags = 0;

	dependencies[1].srcSubpass = 0;
	dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	dependencies[1].dependencyFlags = 0;

	return dependencies;
}



void VRenderPass::createOffScreenRenderPass(VkFormat format, VkFormat depthFormat)
{
	fillOffScreenPositionAttachment(format);
	fillOffScreenColorAttachment(format);
	fillOffScreenNormalAttachment(format);
	fillOffScreenDepthAttachment(depthFormat);

	std::array<VkAttachmentReference, 3> attachmentRefs{
		offscreenPositionAttachmentRef,
		offscreenColorAttachmentRef,
		offscreenNormalAttachmentRef
	};

	offScreenSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	offScreenSubpass.colorAttachmentCount = static_cast<uint32_t>(attachmentRefs.size());
	offScreenSubpass.pColorAttachments = attachmentRefs.data();
	offScreenSubpass.pDepthStencilAttachment = &offscreenDepthAttachmentRef;

	offScreenSubpassDependencies = SetSubpassDependencies();

	std::array<VkAttachmentDescription, 4> offscreenRenderPassAttachments = {
		offscreenPositionAttachment,
		offscreenColorAttachment,
		offscreenNormalAttachment,
		offscreenDepthAttachment
	};

	offscreenRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	offscreenRenderPassInfo.attachmentCount = static_cast<uint32_t>(offscreenRenderPassAttachments.size());	
	offscreenRenderPassInfo.pAttachments = offscreenRenderPassAttachments.data();							
	offscreenRenderPassInfo.subpassCount = 1;													
	offscreenRenderPassInfo.pSubpasses = &offScreenSubpass;										
	offscreenRenderPassInfo.dependencyCount = static_cast<uint32_t>(offScreenSubpassDependencies.size());
	offscreenRenderPassInfo.pDependencies = offScreenSubpassDependencies.data();


	if (vkCreateRenderPass(device, &offscreenRenderPassInfo, nullptr, &offScreenRenderPass) != VK_SUCCESS) {
		throw std::runtime_error("failed to create render pass!");
	}

}

void VRenderPass::fillOffScreenPositionAttachment(VkFormat positionFormat)
{
	offscreenPositionAttachment.format = positionFormat;
	offscreenPositionAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	offscreenPositionAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	offscreenPositionAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	offscreenPositionAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	offscreenPositionAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	offscreenPositionAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	offscreenPositionAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	offscreenPositionAttachmentRef.attachment = 0;
	offscreenPositionAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

void VRenderPass::fillOffScreenColorAttachment(VkFormat colorFormat)
{
	offscreenColorAttachment.format = colorFormat;
	offscreenColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	offscreenColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	offscreenColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	offscreenColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	offscreenColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	offscreenColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	offscreenColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	offscreenColorAttachmentRef.attachment = 1;
	offscreenColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

void VRenderPass::fillOffScreenNormalAttachment(VkFormat normalFormat)
{
	offscreenNormalAttachment.format = normalFormat;
	offscreenNormalAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	offscreenNormalAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	offscreenNormalAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	offscreenNormalAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	offscreenNormalAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	offscreenNormalAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	offscreenNormalAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	offscreenNormalAttachmentRef.attachment = 2;
	offscreenNormalAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

}

void VRenderPass::fillOffScreenDepthAttachment(VkFormat depthFormat)
{
	offscreenDepthAttachment.format = depthFormat;
	offscreenDepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	offscreenDepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	offscreenDepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	offscreenDepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	offscreenDepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	offscreenDepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	offscreenDepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	offscreenDepthAttachmentRef.attachment = 3;
	offscreenDepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
}
