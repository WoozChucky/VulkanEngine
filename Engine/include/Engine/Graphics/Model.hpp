//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_MODEL_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_MODEL_HPP_

#include "Device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace nl {

class Model {

 public:

	struct Vertex {
		glm::vec2 position;
		glm::vec3 color;
		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	Model(Device& device, const std::vector<Vertex>& vertices);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

 private:

	void createVertexBuffers(const std::vector<Vertex>& vertices);

	Device& _device;
 	VkBuffer _vertexBuffer;
	VkDeviceMemory _vertexBufferMemory;
	U32 _vertexCount;
};

}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_MODEL_HPP_
