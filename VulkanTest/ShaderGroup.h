#pragma once
#include <string>
#include <vulkan/vulkan.h>
#include <vector>
#include "Shader.h"
#include "VObject.h"

class ShaderGroup : VObject
{
public:
	ShaderGroup(VkDevice device, std::string vertexFile, std::string fragmentFile);
	

	 VkPipelineShaderStageCreateInfo* getShaderStages();

	void cleanUp();

private:

	Shader vertexShader;
	Shader fragmentShader;

	//0 = vertex
	//1 = fragment
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

};

