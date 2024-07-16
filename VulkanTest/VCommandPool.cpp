#include "VCommandPool.h"
#include <stdexcept>

VCommandPool::VCommandPool(VkDevice device, QueueFamilyIndices indices, VkCommandPoolCreateFlags flags) : device(device)
{
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = flags;
	poolInfo.queueFamilyIndex = indices.graphicsFamily.value();


	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void VCommandPool::cleanUp()
{
	vkDestroyCommandPool(device, commandPool, nullptr);

}




