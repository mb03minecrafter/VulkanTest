#pragma once
#include "Window.h"
#include "VInstance.h"
#include <vector>
#include "VDebugManager.h"
#include "VPhysicalDevice.h"
#include "VDevice.h"
#include "VSurface.h"
#include "VSwapChain.h"
#include "VPipeline.h"
#include "VRenderPass.h"
#include "ShaderGroup.h"
#include "VFrameBufferHandler.h"
#include "VCommandPool.h"
#include "VSyncObjects.h"
#include "VBuffer.h"
#include "Vertex.h"

class Renderer
{
public:

	Renderer();

	bool shouldWindowClose();
	void cleanUp();

	void drawFrame();

	void deviceWaitIdle() {
		vkDeviceWaitIdle(device);
	}

	bool framebufferResized = false;


private:
	
	const int MAX_FRAMES_IN_FLIGHT = 2;


	Window window;
	VInstance instance;
	VDebugManager debugManager;

	VSurface surface;
	VPhysicalDevice physicalDevice;

	VDevice device;
	VSwapChain swapChain;

	ShaderGroup defaultShaderGroup;

	VRenderPass renderPass;
	VPipeline pipeline;


	VFrameBufferHandler frameBufferHandler;

	VCommandPool commandPool;

	VSyncObjects syncObjects;


	std::vector<VkCommandBuffer> commandBuffers;

	void createCommandBuffer();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	uint32_t currentFrame = 0;

	void recreateSwapChain();



	VBuffer* vertexBuffer;

	const std::vector<Vertex> vertices = {
	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};
};


