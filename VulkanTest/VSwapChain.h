#pragma once
#include "VObject.h"
#include <vector>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "VPhysicalDevice.h"
#include "VImage.h"


struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VSwapChain : VObject
{
public:
    VSwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window, QueueFamilyIndices indices);

    void cleanUp();


    void createImageViews();

    
    VkSwapchainKHR getSwapChain() {
        return swapChain;
    }

    VkFormat getSwapChainImageFormat() {
        return swapChainImageFormat;
    }

    VkExtent2D* getSwapChainExtentPtr() {
        return &swapChainExtent;
    }

    VkExtent2D getSwapChainExtent() {
        return swapChainExtent;
    }

    std::vector<VkImageView>* getSwapChainImageViewsPtr() {
        return &swapChainImageViews;
    }



    void recreate();

private:

    VkSwapchainKHR swapChain;

    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR surface;
    GLFWwindow* window;
    QueueFamilyIndices indices;

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);



    void createSwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window, QueueFamilyIndices indices);


    VkExtent2D swapChainExtent;
    VkFormat swapChainImageFormat;

    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;


    std::vector<VImage> swapChainImagesNEW;

};

