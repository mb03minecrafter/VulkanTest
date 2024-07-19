#pragma once
#include <vector>
#include "VBuffer.h"
#include "UniformBufferObject.h"
#include <memory>
#include "Camera.h"
class UniformBufferHandler : VObject
{
public:
	UniformBufferHandler(VmaAllocator allocator, VkExtent2D* swapChainExtent, int MAX_FRAMES_IN_FLIGHT);

	void cleanUp();


	void updateUniformBuffer(uint32_t currentImage, Camera camera);

	std::vector<VBuffer>* getBuffersPtr() {
		return &buffers;
	}

private:
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);


	std::vector<UniformBufferObject> uniforms;

	int MAX_FRAMES_IN_FLIGHT;
	VmaAllocator allocator;
	VkExtent2D* swapChainExtent;
	std::vector<VBuffer> buffers;

};

