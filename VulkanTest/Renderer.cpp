#include "Renderer.h"
#include "Window.h"
#include <stdexcept>




Renderer::Renderer() :
	window(800, 600, (char*)"test", this),
	debugManager(instance.enableValidationLayers, instance),
	surface(instance, window),
	physicalDevice(instance, surface)



{

	device = std::make_unique<VDevice>(instance, physicalDevice, physicalDevice.pickedQueueFamilyIndices);
	swapChain = std::make_unique<VSwapChain>(*device, physicalDevice, surface, window, physicalDevice.pickedQueueFamilyIndices);
	defaultShaderGroup = std::make_unique<ShaderGroup>(*device, "shaders/vert.spv", "shaders/frag.spv");
	descriptorSetLayout = std::make_unique<VDescriptorSetLayout>(*device);


	allocator = std::make_unique<VAllocator>(instance, *device, physicalDevice);
	renderCommandPool = std::make_unique<VCommandPool>(*device, physicalDevice.pickedQueueFamilyIndices, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	copyCommandPool = std::make_unique<VMemTransferCommandPool>(*device, physicalDevice.pickedQueueFamilyIndices, device->getGraphicsQueue());


	renderPass = std::make_unique<VRenderPass>(*device, swapChain->swapChainImageFormat);

	pipeline = std::make_unique<VPipeline>(*device, *renderPass, *defaultShaderGroup, swapChain->swapChainExtent, *descriptorSetLayout);
	frameBufferHandler = std::make_unique<VFrameBufferHandler>(*device, *renderPass, swapChain->swapChainImageViews, swapChain->swapChainExtent);

	syncObjects = std::make_unique<VSyncObjects>(*device, MAX_FRAMES_IN_FLIGHT);


	

	meshBufferHandler = new MeshBufferHandler(*allocator, vertices, indices);
	meshBufferHandler->transferStagingBuffer(*copyCommandPool);

	createCommandBuffer();
}

bool Renderer::shouldWindowClose()
{
	return window.windowShouldClose();
}

void Renderer::cleanUp()
{



	meshBufferHandler->cleanUp();
	delete meshBufferHandler;


	allocator->cleanUp();
	syncObjects->cleanUp();

	copyCommandPool->cleanUp();
	renderCommandPool->cleanUp();

	frameBufferHandler->cleanUp();

	descriptorSetLayout->cleanUp();

	pipeline->cleanUp();
	renderPass->cleanUp();
	swapChain->cleanUp();
	device->cleanUp();
	surface.cleanUp();
	debugManager.cleanUp();
	instance.cleanUp();
	window.cleanUp();
}

void Renderer::drawFrame()
{
	vkWaitForFences(*device, 1, syncObjects->getInFlightFencePtr(currentFrame), VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(*device, *swapChain, UINT64_MAX, syncObjects->getImageAvailableSemaphore(currentFrame), VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(*device, 1, syncObjects->getInFlightFencePtr(currentFrame));


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

	VkSwapchainKHR swapChains[] = { *swapChain };
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

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = *renderPass;
	renderPassInfo.framebuffer = frameBufferHandler->getFrameBuffer(imageIndex);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->swapChainExtent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChain->swapChainExtent.width;
	viewport.height = (float)swapChain->swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain->swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


	VkBuffer vertexBuffers[] = { *meshBufferHandler->vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, *meshBufferHandler->indexBuffer, 0, VK_INDEX_TYPE_UINT16);


	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

}

void Renderer::recreateSwapChain()
{

	int width = 0, height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(*device);

	frameBufferHandler->cleanUp();
	swapChain->cleanUp();

	swapChain->recreate();
	frameBufferHandler->recreate();


}

void Renderer::createCommandBuffer()
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = *renderCommandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(*device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}
