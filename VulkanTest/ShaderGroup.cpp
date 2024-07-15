#include "ShaderGroup.h"

ShaderGroup::ShaderGroup(VkDevice device, std::string vertexFile, std::string fragmentFile) 
	: vertexShader(device, vertexFile, VK_SHADER_STAGE_VERTEX_BIT),
	  fragmentShader(device, fragmentFile, VK_SHADER_STAGE_FRAGMENT_BIT)

{
	shaderStages.push_back(vertexShader.shaderStageInfo);
	shaderStages.push_back(fragmentShader.shaderStageInfo);
}

VkPipelineShaderStageCreateInfo* ShaderGroup::getShaderStages()
{
	return shaderStages.data();
}

void ShaderGroup::cleanUp()
{
	vertexShader.cleanUp();
	fragmentShader.cleanUp();
}


