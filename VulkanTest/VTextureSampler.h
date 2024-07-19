#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>

class VTextureSampler : VObject
{
public:
	VTextureSampler(VkDevice device, VkPhysicalDevice physicalDevice);
	void cleanUp();

	VkSampler getTextureSampler() {
		return textureSampler;
	}

private:


	VkDevice device;

	VkSamplerCreateInfo samplerInfo{};
	VkSampler textureSampler;
};

