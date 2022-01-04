//
// Created by nunol on 12/30/2021.
//

#ifndef GL_GAME_SRC_APPLICATION_HPP_
#define GL_GAME_SRC_APPLICATION_HPP_

#include <Engine/Graphics/Renderer.hpp>
#include "Engine/Graphics/Window.hpp"
#include "Engine/Graphics/Device.hpp"
#include "Engine/Graphics/Model.hpp"
#include "Engine/Graphics/GameObject.hpp"
#include "Engine/Graphics/Descriptors.hpp"

#include <memory>
#include <vector>

class Application {

 public:

	Application();
	~Application();

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	static constexpr S32 WIDTH = 1024;
	static constexpr S32 HEIGHT = 768;

	void run();

 private:
	void loadGameObjects();

	nl::gfx::Window _window {WIDTH, HEIGHT, "Vulkan Practises"};
	nl::gfx::Device _device {_window};
	nl::gfx::Renderer _renderer{_window, _device};

	std::unique_ptr<nl::gfx::DescriptorPool> _globalPool{};
	nl::gfx::GameObject::Map gameObjects;
};

#endif //GL_GAME_SRC_APPLICATION_HPP_
