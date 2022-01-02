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

#include <memory>
#include <vector>

namespace nl::gfx {

class SimpleRenderSystem {

 public:

	SimpleRenderSystem(Device& device, VkRenderPass renderPass);
	~SimpleRenderSystem();

	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

	void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);

 private:

	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);

;
	Device& _device;
	std::unique_ptr<Pipeline> _pipeline;
	VkPipelineLayout _pipelineLayout;
};


}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_SIMPLERENDERSYSTEM_HPP_
