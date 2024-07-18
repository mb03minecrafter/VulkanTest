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
#include "VAllocator.h"
#include "VStagingBuffer.h"
#include "VMemTransferCommandPool.h"
#include "MeshBufferHandler.h"
#include "VDescriptorSetLayout.h"

class Renderer
{
public:

	Renderer();

	bool shouldWindowClose();
	void cleanUp();

	void drawFrame();

	void deviceWaitIdle() {
		vkDeviceWaitIdle(*device);
	}

	bool framebufferResized = false;


private:
	
	const int MAX_FRAMES_IN_FLIGHT = 2;


	Window window;


	VInstance instance;
	VDebugManager debugManager;

	VSurface surface;
	VPhysicalDevice physicalDevice;


	std::unique_ptr<VDevice> device;

	std::unique_ptr<VSwapChain> swapChain;

	std::unique_ptr<ShaderGroup> defaultShaderGroup;


	std::unique_ptr<VRenderPass> renderPass;
	std::unique_ptr<VDescriptorSetLayout> descriptorSetLayout;


	std::unique_ptr<VPipeline> pipeline;


	std::unique_ptr<VFrameBufferHandler> frameBufferHandler;

	std::unique_ptr<VSyncObjects> syncObjects;

	std::unique_ptr<VCommandPool> renderCommandPool;
	std::unique_ptr<VMemTransferCommandPool> copyCommandPool;

	std::unique_ptr<VAllocator> allocator;




	std::vector<VkCommandBuffer> commandBuffers;

	void createCommandBuffer();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	uint32_t currentFrame = 0;

	void recreateSwapChain();



	MeshBufferHandler* meshBufferHandler;


	const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
	};

};


