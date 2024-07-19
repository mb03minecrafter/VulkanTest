#pragma once
#include "VObject.h"
#include "vulkan/vulkan.h"

class VRenderPass : VObject
{

public:
	VRenderPass(VkDevice device, VkFormat colorFormat, VkFormat depthFormat);

	void cleanUp();

	

	VkRenderPass getRenderPass() {
		return renderPass;
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


};

