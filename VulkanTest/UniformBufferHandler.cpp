#include "UniformBufferHandler.h"
#include <gtc/matrix_transform.hpp>
#include <chrono>


UniformBufferHandler::UniformBufferHandler(VmaAllocator allocator, VkExtent2D* swapChainExtent, int MAX_FRAMES_IN_FLIGHT) :
	MAX_FRAMES_IN_FLIGHT(MAX_FRAMES_IN_FLIGHT),
	allocator(allocator),
	swapChainExtent(swapChainExtent)
	
{
	uniforms.resize(MAX_FRAMES_IN_FLIGHT);

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		
		VBuffer buffer(allocator,
			bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VMA_MEMORY_USAGE_AUTO,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT);
		
		buffers.push_back(buffer);


	}




}

void UniformBufferHandler::cleanUp()
{
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		buffers[i].cleanUp();
	}
}

void UniformBufferHandler::updateUniformBuffer(uint32_t currentImage, Camera camera)
{
	

	UniformBufferObject ubo{};
	ubo.view = camera.GetViewMatrix();//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent->width / (float)swapChainExtent->height, 0.1f, 1000.0f);
	ubo.proj[1][1] *= -1;

	memcpy(buffers[currentImage].getMappedData(), &ubo, sizeof(ubo));
}
