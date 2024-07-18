#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "VBuffer.h"
#include <iostream>


class VDescriptorSets : VObject
{
public:
	VDescriptorSets(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, std::vector<VBuffer>& buffers, int MAX_FRAMES_IN_FLIGHT);

	void cleanUp() {
	};

	VkDescriptorSet* getDescriptorSet(int index) {
		

		return &descriptorSets[index];
	}

private:
	std::vector<VkDescriptorSetLayout> layouts;
	std::vector<VkDescriptorSet> descriptorSets;

};

