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
#include "VBuffer.h"
#include "Vertex.h"
#include "VAllocator.h"
#include "VStagingBuffer.h"
#include "VMemTransferCommandPool.h"
#include "MeshBufferHandler.h"
#include "VDescriptorSetLayout.h"
#include "UniformBufferHandler.h"
#include "VDescriptorPool.h"
#include "VDescriptorSets.h"
#include "VTextureImage.h"
#include "VTextureSampler.h"
#include "VDepth.h"
#include "Camera.h"
#include "VFrameDataHandler.h"
#include "DescriptorLayoutBuilder.h"
#include "MarchingCubes.h"

class Renderer
{
public:

	Renderer();

	bool shouldWindowClose();
	void cleanUp();

	void drawFrame();

	void deviceWaitIdle() {
		vkDeviceWaitIdle(device->getDevice());
	}

	bool framebufferResized = false;

	std::unique_ptr<Camera> camera;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
private:
	
	const int MAX_FRAMES_IN_FLIGHT = 2;


	std::unique_ptr<Window> window;
	std::unique_ptr<VInstance> instance;



	std::unique_ptr<VDebugManager> debugManager;

	std::unique_ptr<VSurface> surface;

	std::unique_ptr<VPhysicalDevice> physicalDevice;


	std::unique_ptr<VDevice> device;

	std::unique_ptr<VSwapChain> swapChain;

	std::unique_ptr<ShaderGroup> defaultShaderGroup;


	std::unique_ptr<VRenderPass> renderPass;
	std::unique_ptr<VDescriptorSetLayout> descriptorSetLayout;


	std::unique_ptr<VPipeline> pipeline;


	std::unique_ptr<VFrameBufferHandler> frameBufferHandler;


	std::unique_ptr<VCommandPool> renderCommandPool;
	std::unique_ptr<VMemTransferCommandPool> copyCommandPool;

	std::unique_ptr<VAllocator> allocator;


	std::unique_ptr<UniformBufferHandler> uniformBufferHandler;

	std::unique_ptr<VDescriptorPool> descriptorPool;
	std::unique_ptr<VDescriptorSets> descriptorSets;



	std::unique_ptr<VTextureImage> textureImage;
	std::unique_ptr<VTextureSampler> textureSampler;

	std::unique_ptr<VDepth> depth;

	std::unique_ptr<VFrameDataHandler> frameDataHandler;

	std::unique_ptr <MarchingCubes> MC;
	
	VkDescriptorSetLayout testLayout;
	DescriptorLayoutBuilder builder;

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	uint32_t currentFrame = 0;

	void recreateSwapChain();



	MeshBufferHandler* meshBufferHandler;


	


	void processInput() {
		if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window->getWindowPtr(), true);


		if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
		if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
		if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
		if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

		if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_SPACE) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera_Movement::UP, deltaTime);

		if (glfwGetKey(window->getWindowPtr(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			camera->ProcessKeyboard(Camera_Movement::DOWN, deltaTime);




	}

};


