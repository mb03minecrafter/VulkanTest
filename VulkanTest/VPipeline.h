#pragma once
#include "VObject.h"
#include "Shader.h"
#include "vulkan/vulkan.h"
#include "ShaderGroup.h"

class VPipeline : VObject
{
public:
	VPipeline(VkDevice device, VkRenderPass renderPass, ShaderGroup shaderGroup, VkExtent2D swapChainExtent);

	void cleanUp();

	operator VkPipeline() const {
		return graphicsPipeline;
	}

private:

	VkDevice device;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	VkPipeline graphicsPipeline;



	const std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};


	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	VkPipelineLayout pipelineLayout;




	VkPipelineDynamicStateCreateInfo dynamicState{};
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};


	VkViewport viewport{};
	VkRect2D scissor{};
	VkPipelineViewportStateCreateInfo viewportState{};

	VkPipelineRasterizationStateCreateInfo rasterizer{};

	VkPipelineMultisampleStateCreateInfo multisampling{};

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	VkPipelineColorBlendStateCreateInfo colorBlending{};

	void configDynamicState();
	void configVertexInputInfo();
};

