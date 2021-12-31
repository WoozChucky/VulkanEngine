//
// Created by nunol on 12/31/2021.
//

#include "Engine/Graphics/SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>

struct SimplePushConstantData {
	glm::mat2 transform{1.f};
	glm::vec2 offset;
	alignas(16) glm::vec3 color;
};

nl::SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass) : _device{device} {
	createPipelineLayout();
	createPipeline(renderPass);
}

nl::SimpleRenderSystem::~SimpleRenderSystem() {
	vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
}

void nl::SimpleRenderSystem::createPipelineLayout() {

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

void nl::SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {;
	assert(_pipelineLayout != nullptr);

	nl::PipelineConfigInfo pipelineConfig{};

	Pipeline::defaultPipelineConfigInfo(pipelineConfig);

	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = _pipelineLayout;

	_pipeline = std::make_unique<Pipeline>(_device,
										   "../../Game/Shaders/simple_shader.vert.spv",
										   "../../Game/Shaders/simple_shader.frag.spv",
										   pipelineConfig);

}

void nl::SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject> gameObjects) {

	_pipeline->bind(commandBuffer);

	for(auto& obj : gameObjects) {
		obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + .01f, glm::two_pi<R32>());

		SimplePushConstantData push{};
		push.offset =obj.transform2d.translation;
		push.color = obj.color;
		push.transform = obj.transform2d.mat2();

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
