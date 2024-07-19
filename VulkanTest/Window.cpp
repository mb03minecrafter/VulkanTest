#include "Window.h"
#include <iostream>
#include "Renderer.h"
#include <trigonometric.hpp>


float lastX = 400, lastY = 300;
bool firstMouse = true;

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->framebufferResized = true;
}

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    auto renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));
    
    renderer->camera->ProcessMouseMovement(xoffset, yoffset);

    /*renderer->camera->Yaw += xoffset;
    renderer->camera->Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (renderer->camera->Pitch > 89.0f)
        renderer->camera->Pitch = 89.0f;
    if (renderer->camera->Pitch < -89.0f)
        renderer->camera->Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(renderer->camera->Yaw)) * cos(glm::radians(renderer->camera->Pitch));
    front.y = sin(glm::radians(renderer->camera->Pitch));
    front.z = sin(glm::radians(renderer->camera->Yaw)) * cos(glm::radians(renderer->camera->Pitch));
    renderer->camera->Front = glm::normalize(front);
    renderer->camera->Right = glm::normalize(glm::cross(renderer->camera->Front, renderer->camera->WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    renderer->camera->Up = glm::normalize(glm::cross(renderer->camera->Right, renderer->camera->Front));*/
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto renderer = reinterpret_cast<Renderer*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_W) {
        renderer->camera->ProcessKeyboard(Camera_Movement::FORWARD, renderer->deltaTime);
    }
    if (key == GLFW_KEY_S) {
        renderer->camera->ProcessKeyboard(Camera_Movement::BACKWARD, renderer->deltaTime);

    }
    if (key == GLFW_KEY_A) {
        renderer->camera->ProcessKeyboard(Camera_Movement::LEFT, renderer->deltaTime);

    }
    if (key == GLFW_KEY_D) {
        renderer->camera->ProcessKeyboard(Camera_Movement::RIGHT, renderer->deltaTime);

    }


}



Window::Window(int width, int height, char* title, Renderer* renderer)
{

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwSetWindowUserPointer(window, renderer);

	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetKeyCallback(window, key_callback);
}

void Window::cleanUp()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::windowShouldClose()
{
	return glfwWindowShouldClose(window);
}
