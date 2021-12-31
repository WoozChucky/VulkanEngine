//
// Created by nunol on 12/30/2021.
//

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <vector>
#include <array>
#include "Application.hpp"
#include "Engine/Graphics/SimpleRenderSystem.hpp"

nl::Application::Application() {
	loadGameObjects();
}

nl::Application::~Application() { }

void nl::Application::run() {

	SimpleRenderSystem simpleRenderSystem(_device, _renderer.getSwapChainRenderPass());

	while (!_window.shouldClose()) {

		glfwPollEvents();

		if (auto commandBuffer = _renderer.beginFrame()) {

			_renderer.beginSwapChainRenderPass(commandBuffer);

			simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);

			_renderer.endSwapChainRenderPass(commandBuffer);
			_renderer.endFrame();
		}

	}

	vkDeviceWaitIdle(_device.device());
}

void nl::Application::loadGameObjects() {

	std::vector<Model::Vertex> vertices {
		{{0.0f, -0.5f}, {1.0, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	auto model = std::make_shared<Model>(_device, vertices);

	auto triangle = GameObject::createGameObject();
	triangle.model = model;
	triangle.color = {.1f, .8f, .1f};
	triangle.transform2d.translation.x = .2f;
	triangle.transform2d.scale = {2.f, .5f};
	triangle.transform2d.rotation = .25f * glm::two_pi<R32>();

	gameObjects.push_back(std::move(triangle));
}
