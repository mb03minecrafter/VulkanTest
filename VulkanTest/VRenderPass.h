#pragma once
#include "VObject.h"
#include "vulkan/vulkan.h"

class VRenderPass : VObject
{

public:
	VRenderPass(VkDevice device, VkFormat format);

	void cleanUp();

	operator VkRenderPass() const {
		return renderPass;
	}

private:



	VkDevice device;

	VkRenderPassCreateInfo renderPassInfo{};
	VkRenderPass renderPass;

	VkAttachmentDescription colorAttachment{};
	VkAttachmentReference colorAttachmentRef{};
	VkSubpassDescription subpass{};


};

