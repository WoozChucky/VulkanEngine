//
// Created by nunol on 12/30/2021.
//

#include <vector>
#include <array>
#include <chrono>

#include "Application.hpp"

#include "Engine/Systems/SimpleRenderSystem.hpp"
#include "Engine/Systems/PointLightSystem.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Engine/Input/KeyboardMovementController.hpp"

struct GlobalUbo {
	glm::mat4 projection{1.f};
	glm::mat4 view{1.f};
	glm::vec4 ambientLightColor{1.f, 1.f, 1.f, .02f};  // w is intensity
	glm::vec3 lightPosition{-1.f};
	alignas(16) glm::vec4 lightColor{1.f};  // w is light intensity
};

Application::Application() {
	_globalPool = nl::gfx::DescriptorPool::Builder(_device)
		.setMaxSets(nl::gfx::SwapChain::MAX_FRAMES_IN_FLIGHT)
		.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, nl::gfx::SwapChain::MAX_FRAMES_IN_FLIGHT)
		.build();
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
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
		);
		uboBuffers[i]->map();
	}

	auto globalSetLayout = nl::gfx::DescriptorSetLayout::Builder(_device)
		.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
		.build();

	std::vector<VkDescriptorSet> globalDescriptorSets(nl::gfx::SwapChain::MAX_FRAMES_IN_FLIGHT);
	for (int i = 0; i < globalDescriptorSets.size(); ++i) {
		auto bufferInfo = uboBuffers[i]->descriptorInfo();
		nl::gfx::DescriptorWriter(*globalSetLayout, *_globalPool)
		.writeBuffer(0, &bufferInfo)
		.build(globalDescriptorSets[i]);
	}

	nl::sys::SimpleRenderSystem simpleRenderSystem(
		_device,
		_renderer.getSwapChainRenderPass(),
		globalSetLayout->getDescriptorSetLayout()
	);

	nl::sys::PointLightSystem pointLightSystem(
		_device,
		_renderer.getSwapChainRenderPass(),
		globalSetLayout->getDescriptorSetLayout()
	);

	nl::gfx::Camera camera{};

	auto viewerObject = nl::gfx::GameObject::createGameObject();
	viewerObject.transform.translation.z = -2.5f;
	nl::io::KeyboardMovementController cameraController{};

	auto currentTime = std::chrono::high_resolution_clock::now();

	while (!_window.shouldClose()) {
		glfwPollEvents();

		auto newTime = std::chrono::high_resolution_clock::now();
		R32 frameTime = std::chrono::duration<R32, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		// frameTime = glm::min(frameTime, 60.f);

		cameraController.moveInPlaneXZ(_window.getGLFWwindow(), frameTime, viewerObject);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

		R32 aspect = _renderer.getAspectRation();
		camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

		if (auto commandBuffer = _renderer.beginFrame()) {
			S32 frameIndex = _renderer.getFrameIndex();
			nl::gfx::FrameInfo frameInfo {
				frameIndex,
				frameTime,
				commandBuffer,
				camera,
				globalDescriptorSets[frameIndex],
				gameObjects
			};

			// Update Game Objects
			GlobalUbo ubo {};
			ubo.projection = camera.getProjection();
			ubo.view = camera.getView();
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();

			// Render Game Objects
			_renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(frameInfo);
			pointLightSystem.render(frameInfo);
			_renderer.endSwapChainRenderPass(commandBuffer);
			_renderer.endFrame();
		}

	}

	vkDeviceWaitIdle(_device.device());
}

void Application::loadGameObjects() {

	std::shared_ptr<nl::gfx::Model> model =
		nl::gfx::Model::createModelFromFile(_device, "../../Game/Models/flat_vase.obj");
	auto flatVase = nl::gfx::GameObject::createGameObject();
	flatVase.model = model;
	flatVase.transform.translation = {-.5f, .5f, 0.f};
	flatVase.transform.scale = {3.f, 1.5f, 3.f};
	gameObjects.emplace(flatVase.getId(), std::move(flatVase));

	model = nl::gfx::Model::createModelFromFile(_device, "../../Game/Models/smooth_vase.obj");
	auto smoothVase = nl::gfx::GameObject::createGameObject();
	smoothVase.model = model;
	smoothVase.transform.translation = {.5f, .5f, 0.f};
	smoothVase.transform.scale = {3.f, 1.5f, 3.f};
	gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

	model = nl::gfx::Model::createModelFromFile(_device, "../../Game/Models/quad.obj");
	auto floor = nl::gfx::GameObject::createGameObject();
	floor.model = model;
	floor.transform.translation = {0.f, .5f, 0.f};
	floor.transform.scale = {3.f, 1.f, 3.f};
	gameObjects.emplace(floor.getId(), std::move(floor));
}
