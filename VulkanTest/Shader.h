#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "VObject.h"
class Shader : VObject
{

public:
	Shader(VkDevice device, std::string& filename, VkShaderStageFlagBits stage);

	void cleanUp();

	operator VkShaderModule() const {
		return shaderModule;
	}
	VkPipelineShaderStageCreateInfo shaderStageInfo;


private:
	VkDevice device;
	VkShaderModule shaderModule;
	std::vector<char> readFile(const std::string& filename);
};

