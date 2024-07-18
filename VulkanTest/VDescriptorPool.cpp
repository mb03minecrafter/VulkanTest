#include "VDescriptorPool.h"
#include <stdexcept>

VDescriptorPool::VDescriptorPool(VkDevice device, int MAX_FRAMES_IN_FLIGHT) : device(device)
{
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
	
}

void VDescriptorPool::cleanUp()
{
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
}
