#include "Renderer.h"
#include "Window.h"
#include <stdexcept>
#include <array>
#include <chrono>



Renderer::Renderer() 
{

	camera = std::make_unique<Camera>();

	window = std::make_unique<Window>(800, 600, (char*)"test", this);

	instance = std::make_unique<VInstance>();
	debugManager = std::make_unique<VDebugManager>(instance->enableValidationLayers, instance->getInstance());

	surface = std::make_unique<VSurface>(instance->getInstance(), window->getWindowPtr());
	physicalDevice = std::make_unique<VPhysicalDevice>(instance->getInstance(), surface->getSurface());

	device = std::make_unique<VDevice>(*instance, physicalDevice->getPhysicalDevice(), physicalDevice->pickedQueueFamilyIndices);
	swapChain = std::make_unique<VSwapChain>(device->getDevice(), physicalDevice->getPhysicalDevice(), surface->getSurface(), window->getWindowPtr(), physicalDevice->pickedQueueFamilyIndices);
	defaultShaderGroup = std::make_unique<ShaderGroup>(device->getDevice(), "shaders/vert.spv", "shaders/frag.spv");
	descriptorSetLayout = std::make_unique<VDescriptorSetLayout>(device->getDevice());


	allocator = std::make_unique<VAllocator>(instance->getInstance(), device->getDevice(), physicalDevice->getPhysicalDevice());
	renderCommandPool = std::make_unique<VCommandPool>(device->getDevice(), physicalDevice->pickedQueueFamilyIndices, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	copyCommandPool = std::make_unique<VMemTransferCommandPool>(device->getDevice(), physicalDevice->pickedQueueFamilyIndices, device->getGraphicsQueue());

	depth = std::make_unique<VDepth>(device->getDevice(), physicalDevice->getPhysicalDevice(), allocator->getAllocator(), swapChain->getSwapChainExtentPtr());

	renderPass = std::make_unique<VRenderPass>(device->getDevice(), swapChain->getSwapChainImageFormat(), depth->getDepthFormat());

	pipeline = std::make_unique<VPipeline>(device->getDevice(), renderPass->getRenderPass(), *defaultShaderGroup, descriptorSetLayout->getDescriptorSetLayout());
	frameBufferHandler = std::make_unique<VFrameBufferHandler>(device->getDevice(), renderPass->getRenderPass(), renderPass->getOffscreenRenderPass(), swapChain->getSwapChainImageViewsPtr(), swapChain->getSwapChainExtentPtr(), depth->getDepthImageViewPtr());

	syncObjects = std::make_unique<VSyncObjects>(device->getDevice(), MAX_FRAMES_IN_FLIGHT);

	uniformBufferHandler = std::make_unique<UniformBufferHandler>(allocator->getAllocator(), swapChain->getSwapChainExtentPtr(), MAX_FRAMES_IN_FLIGHT);
	

	textureImage = std::make_unique<VTextureImage>(device->getDevice(), "textures/brick.png", allocator->getAllocator(), (*copyCommandPool));

	textureSampler = std::make_unique<VTextureSampler>(device->getDevice(), physicalDevice->getPhysicalDevice());

	descriptorPool = std::make_unique<VDescriptorPool>(device->getDevice(), MAX_FRAMES_IN_FLIGHT);
	descriptorSets = std::make_unique<VDescriptorSets>(device->getDevice(), descriptorPool->getDescriptorPool(), descriptorSetLayout->getDescriptorSetLayout(), uniformBufferHandler->getBuffersPtr(), textureImage->getTextureImageView(), textureSampler->getTextureSampler(), MAX_FRAMES_IN_FLIGHT);


	


	meshBufferHandler = new MeshBufferHandler(allocator->getAllocator(), vertices, indices);
	meshBufferHandler->transferStagingBuffer(*copyCommandPool);

	createCommandBuffer();
}

bool Renderer::shouldWindowClose()
{
	return window->windowShouldClose();
}

void Renderer::cleanUp()
{



	meshBufferHandler->cleanUp();
	delete meshBufferHandler;


	textureSampler->cleanUp();
	textureImage->cleanUp();

	depth->cleanUp();

	descriptorPool->cleanUp();

	syncObjects->cleanUp();

	copyCommandPool->cleanUp();
	renderCommandPool->cleanUp();

	frameBufferHandler->cleanUp();

	uniformBufferHandler->cleanUp();

	descriptorSetLayout->cleanUp();


	allocator->cleanUp();

	pipeline->cleanUp();
	renderPass->cleanUp();
	swapChain->cleanUp();
	device->cleanUp();
	surface->cleanUp();
	debugManager->cleanUp();
	instance->cleanUp();
	window->cleanUp();
}

void Renderer::drawFrame()
{


	float currentFrameTime = static_cast<float>(glfwGetTime());
	deltaTime = currentFrameTime - lastFrame;
	lastFrame = currentFrameTime;


	processInput();

	vkWaitForFences(device->getDevice(), 1, syncObjects->getInFlightFencePtr(currentFrame), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device->getDevice(), swapChain->getSwapChain(), UINT64_MAX, syncObjects->getImageAvailableSemaphore(currentFrame), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	uniformBufferHandler->updateUniformBuffer(currentFrame, *camera);


	vkResetFences(device->getDevice(), 1, syncObjects->getInFlightFencePtr(currentFrame));


	vkResetCommandBuffer(commandBuffers[currentFrame], /*VkCommandBufferResetFlagBits*/ 0);
	recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncObjects->getImageAvailableSemaphore(currentFrame) };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { syncObjects->getRenderFinishedSemaphore(currentFrame)};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, syncObjects->getInFlightFence(currentFrame)) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain->getSwapChain()};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;


}

void Renderer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	std::array<VkClearValue, 3> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass->getRenderPass();
	renderPassInfo.framebuffer = frameBufferHandler->getFrameBuffer(imageIndex);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipeline());

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChain->getSwapChainExtentPtr()->width;
	viewport.height = (float)swapChain->getSwapChainExtentPtr()->height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = (swapChain->getSwapChainExtent());
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


	VkBuffer vertexBuffers[] = { *meshBufferHandler->vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, *meshBufferHandler->indexBuffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getPipelineLayout(), 0, 1, descriptorSets->getDescriptorSet(currentFrame), 0, nullptr);



	//using push constants for model matrix, faster
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	meshPushConstant constant;
	constant.model = glm::rotate(glm::mat4(1.0f),  time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	vkCmdPushConstants(commandBuffer, pipeline->getPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(meshPushConstant), &constant);
	//end push constants


	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

}

void Renderer::recreateSwapChain()
{

	int width = 0, height = 0;
	glfwGetFramebufferSize(window->getWindowPtr(), &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window->getWindowPtr(), &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device->getDevice());

	depth->cleanUp();
	frameBufferHandler->cleanUp();
	swapChain->cleanUp();

	swapChain->recreate();
	depth->recreate();
	frameBufferHandler->recreate();


}

void Renderer::createCommandBuffer()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = renderCommandPool->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device->getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}
