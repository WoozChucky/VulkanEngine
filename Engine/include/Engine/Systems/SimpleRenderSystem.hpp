//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_PTLIGHTSYSTEM_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_PTLIGHTSYSTEM_HPP_

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

class SimpleRenderSystem {

 public:

	SimpleRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
	~SimpleRenderSystem();

	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

	void renderGameObjects(FrameInfo& frameInfo);

 private:

	void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
	void createPipeline(VkRenderPass renderPass);

;
	Device& _device;
	std::unique_ptr<Pipeline> _pipeline;
	VkPipelineLayout _pipelineLayout;
};


}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_PTLIGHTSYSTEM_HPP_
