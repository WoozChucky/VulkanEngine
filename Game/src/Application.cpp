//
// Created by nunol on 12/30/2021.
//

#include <vector>
#include <array>
#include <chrono>

#include "Application.hpp"

#include "Engine/Graphics/SimpleRenderSystem.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Engine/Input/KeyboardMovementController.hpp"

struct GlobalUbo {
	glm::mat4 projectionView {1.f};
	glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
};

Application::Application() {
	loadGameObjects();
}

Application::~Application() { }

void Application::run() {

	std::vector<std::unique_ptr<nl::gfx::Buffer>> uboBuffers(nl::gfx::SwapChain::MAX_FRAMES_IN_FLIGHT);

	for (int i = 0; i < uboBuffers.size(); ++i) {
		uboBuffers[i] = std::make_unique<nl::gfx::Buffer>(
			_device,
			sizeof(GlobalUbo),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
		uboBuffers[i]->map();
	}

	nl::gfx::SimpleRenderSystem simpleRenderSystem(_device, _renderer.getSwapChainRenderPass());
	nl::gfx::Camera camera{};

	auto viewerObject = nl::gfx::GameObject::createGameObject();
	nl::io::KeyboardMovementController cameraController{};

	auto currentTime = std::chrono::high_resolution_clock::now();

	while (!_window.shouldClose()) {
		glfwPollEvents();

		auto newTime = std::chrono::high_resolution_clock::now();
		R32 frameTime = std::chrono::duration<R32, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		frameTime = glm::min(frameTime, 60.f);

		cameraController.moveInPlaneXZ(_window.getGLFWwindow(), frameTime, viewerObject);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

		R32 aspect = _renderer.getAspectRation();
		camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);

		if (auto commandBuffer = _renderer.beginFrame()) {
			S32 frameIndex = _renderer.getFrameIndex();
			nl::gfx::FrameInfo frameInfo {
				frameIndex,
				frameTime,
				commandBuffer,
				camera
			};

			// Update Game Objects
			GlobalUbo ubo {};
			ubo.projectionView = camera.getProjection() * camera.getView();
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			// Render Game Objects
			_renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
			_renderer.endSwapChainRenderPass(commandBuffer);
			_renderer.endFrame();
		}

	}

	vkDeviceWaitIdle(_device.device());
}

void Application::loadGameObjects() {

	/*
	std::shared_ptr<nl::gfx::Model> model = nl::gfx::Model::createModelFromFile(_device, "../../Game/Models/smooth_vase.obj");
	auto gameObj = nl::gfx::GameObject::createGameObject();
	gameObj.model = model;
	gameObj.transform.translation = {.5f, .5f, 2.5f};
	gameObj.transform.scale = {3.f, 1.5f, 3.f};

	gameObjects.push_back(std::move(gameObj));
	*/

	std::shared_ptr<nl::gfx::Model> lveModel =
		nl::gfx::Model::createModelFromFile(_device, "../../Game/Models/flat_vase.obj");
	auto flatVase = nl::gfx::GameObject::createGameObject();
	flatVase.model = lveModel;
	flatVase.transform.translation = {-.5f, .5f, 2.5f};
	flatVase.transform.scale = {3.f, 1.5f, 3.f};
	gameObjects.push_back(std::move(flatVase));

	lveModel = nl::gfx::Model::createModelFromFile(_device, "../../Game/Models/smooth_vase.obj");
	auto smoothVase = nl::gfx::GameObject::createGameObject();
	smoothVase.model = lveModel;
	smoothVase.transform.translation = {.5f, .5f, 2.5f};
	smoothVase.transform.scale = {3.f, 1.5f, 3.f};
	gameObjects.push_back(std::move(smoothVase));
}
