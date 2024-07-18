#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
class VDescriptorSetLayout : VObject
{

public:
	VDescriptorSetLayout(VkDevice device);
	void cleanUp();

	operator VkDescriptorSetLayout() const {
		return descriptorSetLayout;
	}

private:

	VkDevice device;

	VkDescriptorSetLayoutBinding uboLayoutBinding{};


	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayoutCreateInfo layoutInfo{};

};

