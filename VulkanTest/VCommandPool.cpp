#include "VCommandPool.h"
#include <stdexcept>

VCommandPool::VCommandPool(VkDevice device, QueueFamilyIndices indices) : device(device)
{
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void VCommandPool::cleanUp()
{
	vkDestroyCommandPool(device, commandPool, nullptr);

}




