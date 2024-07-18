#pragma once
#include <vulkan/vulkan.h>
#include "VObject.h"
#include <vector>

class VInstance : VObject
{
public:
	VInstance();

	void cleanUp();
	void queryExtensions();

#ifdef NDEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = true;
#endif

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};


	

	VkInstance getInstance() {
		return instance;
	}


private:
	VkInstance instance;
	VkApplicationInfo appInfo{};
	VkInstanceCreateInfo createInfo{};
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();



	




};

