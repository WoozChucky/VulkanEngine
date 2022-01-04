//
// Created by nunol on 12/31/2021.
//

#include "Engine/Systems/PointLightSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>

using namespace nl::gfx;
using namespace nl::sys;

PointLightSystem::PointLightSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout) : _device{device} {
	createPipelineLayout(globalSetLayout);
	createPipeline(renderPass);
}

PointLightSystem::~PointLightSystem() {
	vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
}

void PointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {

	// VkPushConstantRange pushConstantRange;
	// pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	// pushConstantRange.offset = 0;
	// pushConstantRange.size = sizeof(SimplePushConstantData);

	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

	VkPipelineLayoutCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineInfo.setLayoutCount = static_cast<U32>(descriptorSetLayouts.size());
	pipelineInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineInfo.pushConstantRangeCount = 0;
	pipelineInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(_device.device(), &pipelineInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create pipeline layout");
	}
}

void PointLightSystem::createPipeline(VkRenderPass renderPass) {;
	assert(_pipelineLayout != nullptr);

	PipelineConfigInfo pipelineConfig{};

	Pipeline::defaultPipelineConfigInfo(pipelineConfig);

	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = _pipelineLayout;
	pipelineConfig.attributeDescriptions.clear();
	pipelineConfig.bindingDescriptions.clear();

	_pipeline = std::make_unique<Pipeline>(_device,
										   "../../Game/Shaders/point_light.vert.spv",
										   "../../Game/Shaders/point_light.frag.spv",
										   pipelineConfig);

}

void PointLightSystem::render(FrameInfo& frameInfo) {

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

	vkCmdDraw(frameInfo.buffer, 6, 1, 0, 0);

}
