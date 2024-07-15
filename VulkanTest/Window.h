#pragma once
#include <GLFW/glfw3.h>

class Renderer;

class Window
{
public:
	Window(int width, int height, char* title, Renderer* renderer);
	void cleanUp();
	bool windowShouldClose();

	operator GLFWwindow* () const {
		return window;
	}

private:
	GLFWwindow* window;
	

};

