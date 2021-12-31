//
// Created by nunol on 12/30/2021.
//

#ifndef GL_GAME_SRC_WINDOW_HPP_
#define GL_GAME_SRC_WINDOW_HPP_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Abstractions/DataTypes.hpp>
#include <string>

namespace nl {

class Window {

 public:
	Window(S32 w, S32 h, std::string  name);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	inline bool shouldClose() { return glfwWindowShouldClose(_window); }
	inline VkExtent2D getExtent() { return { static_cast<U32>(width), static_cast<U32>(height)}; }
	inline bool wasWindowResized() { return _frameBufferResized; }
	inline void resetWindowResized() { _frameBufferResized = false; }

	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

 private:

	static void frameBufferResized(GLFWwindow* window, int width, int height);

	void initWindow();

	S32 width;
	S32 height;
	bool _frameBufferResized = false;

	std::string windowName;
	GLFWwindow* _window;

};

}

#endif //GL_GAME_SRC_WINDOW_HPP_
