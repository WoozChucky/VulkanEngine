//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_SIMPLERENDERSYSTEM_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_SIMPLERENDERSYSTEM_HPP_

#include "Engine/Graphics/Pipeline.hpp"
#include "Engine/Graphics/Device.hpp"
#include "Engine/Graphics/Model.hpp"
#include "Engine/Graphics/GameObject.hpp"
#include "Engine/Graphics/Camera.hpp"
#include "Engine/Graphics/FrameInfo.hpp"

#include <memory>
#include <vector>

namespace nl::sys {

using namespace nl::gfx;

class PointLightSystem {

 public:

	PointLightSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
	~PointLightSystem();

	PointLightSystem(const PointLightSystem&) = delete;
	PointLightSystem& operator=(const PointLightSystem&) = delete;

	void render(FrameInfo& frameInfo);

 private:

	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void createPipeline(VkRenderPass renderPass);

;
	Device& _device;
	std::unique_ptr<Pipeline> _pipeline;
	VkPipelineLayout _pipelineLayout;
};


}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_SIMPLERENDERSYSTEM_HPP_
