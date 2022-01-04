//
// Created by nunol on 12/31/2021.
//

#include "Engine/Systems/SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>

using namespace nl::gfx;
using namespace nl::sys;

struct SimplePushConstantData {
	glm::mat4 modelMatrix{1.f};
	glm::mat4 normalMatrix{1.f};
};

SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : _device{device} {
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
	vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {

	VkPushConstantRange pushConstantRange;
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

	VkPipelineLayoutCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineInfo.setLayoutCount = static_cast<U32>(descriptorSetLayouts.size());
	pipelineInfo.pSetLayouts = descriptorSetLayouts.data();
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

void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo) {

	_pipeline->bind(frameInfo.buffer);

	vkCmdBindDescriptorSets(
		frameInfo.buffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		_pipelineLayout,
		0,
		1,
		&frameInfo.globalDescriptorSet,
		0,
		nullptr
	);

	for(auto& kv : frameInfo.gameObjects) {
		auto& obj = kv.second;

		if (obj.model == nullptr) continue;

		SimplePushConstantData push{};
		push.modelMatrix = obj.transform.mat4();
		push.normalMatrix = obj.transform.normalMatrix();

		vkCmdPushConstants(frameInfo.buffer,
						   _pipelineLayout,
						   VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0,
						   sizeof(SimplePushConstantData),
						   &push
		);

		obj.model->bind(frameInfo.buffer);
		obj.model->draw(frameInfo.buffer);
	}

}
