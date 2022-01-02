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

Application::Application() {
	loadGameObjects();
}

Application::~Application() { }

void Application::run() {

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

			_renderer.beginSwapChainRenderPass(commandBuffer);

			simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

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
