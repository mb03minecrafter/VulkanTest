#pragma once
#include <iostream>
#include "Renderer.h"
#include "Vertex.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"



Renderer renderer;




void mainLoop() {
    while (!renderer.shouldWindowClose()) {
        glfwPollEvents();
        renderer.drawFrame();

    }

    renderer.deviceWaitIdle();
}

int main() {
    
    mainLoop();

    renderer.cleanUp();
    
    return 0;
}

