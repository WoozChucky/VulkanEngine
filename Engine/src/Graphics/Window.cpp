//
// Created by nunol on 12/30/2021.
//

#include "Engine/Graphics/Window.hpp"
#include "Abstractions/Environment.hpp"

#include <utility>
#include <stdexcept>

using namespace nl::gfx;

Window::Window(S32 w, S32 h, std::string name):
	height(h), width(w), windowName(std::move(name)), _window(nullptr) {
	initWindow();
}

Window::~Window() {
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void Window::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, frameBufferResized);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
	if (glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface");
	}

}

void Window::frameBufferResized(GLFWwindow *window, int width, int height) {

	auto selfWindow = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));

	selfWindow->_frameBufferResized = true;
	selfWindow->width = width;
	selfWindow->height = height;
}
