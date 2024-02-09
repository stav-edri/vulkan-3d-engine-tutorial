#include "ke_window.hpp"



namespace ke {
	KeWindow::KeWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{ name } {
		initWindow();
	}

	KeWindow::~KeWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void KeWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface");
		}
	}

	void KeWindow::frameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto keWindow = reinterpret_cast<KeWindow*>(glfwGetWindowUserPointer(window));
		keWindow->frameBufferResized = true;
		keWindow->width = width;
		keWindow->height = height;
	}

	void KeWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No OpenGL context
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // resizable!

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
	}
}