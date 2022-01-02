//
// Created by nunol on 12/31/2021.
//

#include "Engine/Graphics/SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>

using namespace nl::gfx;

struct SimplePushConstantData {
	glm::mat4 transform{1.f};
	glm::mat4 normalMatrix{1.f};
};

SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass) : _device{device} {
	createPipelineLayout();
	createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
	vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout() {

	VkPushConstantRange pushConstantRange;
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineInfo.setLayoutCount = 0;
	pipelineInfo.pSetLayouts = nullptr;
	pipelineInfo.pushConstantRangeCount = 1;
	pipelineInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(_device.device(), &pipelineInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create pipeline layout");
	}
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {;
	assert(_pipelineLayout != nullptr);

	PipelineConfigInfo pipelineConfig{};

	Pipeline::defaultPipelineConfigInfo(pipelineConfig);

	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = _pipelineLayout;

	_pipeline = std::make_unique<Pipeline>(_device,
										   "../../Game/Shaders/simple_shader.vert.spv",
										   "../../Game/Shaders/simple_shader.frag.spv",
										   pipelineConfig);

}

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera) {

	_pipeline->bind(commandBuffer);

	auto projectionView = camera.getProjection() * camera.getView();

	for(auto& obj : gameObjects) {
		SimplePushConstantData push{};
		auto modelMatrix = obj.transform.mat4();
		push.transform = projectionView * modelMatrix;
		push.normalMatrix = obj.transform.normalMatrix();

		vkCmdPushConstants(commandBuffer,
						   _pipelineLayout,
						   VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
						   sizeof(SimplePushConstantData),
						   &push
		);

		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}

}
