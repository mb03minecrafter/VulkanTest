#pragma once
#include "VObject.h"
#include <vulkan/vulkan.h>
#include "VPhysicalDevice.h"
#include "VInstance.h"

class VDevice : VObject
{
public:
	VDevice(VInstance instance, VkPhysicalDevice physicalDevice, QueueFamilyIndices indices);
	void cleanUp();


	operator VkDevice() const {
		return device;
	}

	VkQueue getGraphicsQueue() {
		return graphicsQueue;
	}

	VkQueue getPresentQueue() {
		return presentQueue;
	}

private:
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

};

