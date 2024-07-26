#pragma once
#include "VObject.h"
#include "vulkan/vulkan.h"
#include <array>

class VRenderPass : VObject
{

public:
	VRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat);

	void cleanUp();

	

	VkRenderPass getRenderPass() {
		return renderPass;
	}

	VkRenderPass getOffscreenRenderPass() {
		return offScreenRenderPass;
	}

private:



	VkDevice device;

	VkRenderPassCreateInfo renderPassInfo{};

	VkRenderPass renderPass;

	VkAttachmentDescription colorAttachment{};
	VkAttachmentReference colorAttachmentRef{};

	VkAttachmentDescription depthAttachment{};
	VkAttachmentReference depthAttachmentRef{};

	



	VkSubpassDescription subpass{};

	void fillColorAttachment(VkFormat colorFormat);
	void fillDepthAttachment(VkFormat depthFormat);

	std::array<VkSubpassDependency, 1> SetSubpassDependencies();


	VkRenderPassCreateInfo offscreenRenderPassInfo{};
	VkRenderPass offScreenRenderPass;
	VkSubpassDescription offScreenSubpass;

	std::array<VkSubpassDependency, 1> offScreenSubpassDependencies;



	void createOffScreenRenderPass(VkFormat format, VkFormat depthFormat);

	void fillOffScreenPositionAttachment(VkFormat positionFormat);
	void fillOffScreenColorAttachment(VkFormat colorFormat);
	void fillOffScreenNormalAttachment(VkFormat normalFormat);
	void fillOffScreenDepthAttachment(VkFormat depthFormat);

	VkAttachmentDescription offscreenPositionAttachment{};
	VkAttachmentReference offscreenPositionAttachmentRef{};

	VkAttachmentDescription offscreenColorAttachment{};
	VkAttachmentReference offscreenColorAttachmentRef{};

	VkAttachmentDescription offscreenNormalAttachment{};
	VkAttachmentReference offscreenNormalAttachmentRef{};

	VkAttachmentDescription offscreenDepthAttachment{};
	VkAttachmentReference offscreenDepthAttachmentRef{};


};

