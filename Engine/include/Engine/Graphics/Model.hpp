//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_MODEL_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_MODEL_HPP_

#include "Device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>

namespace nl::gfx {

class Model {

 public:

	struct Vertex {
		glm::vec3 position{};
		glm::vec3 color{};
		glm::vec3 normal{};
		glm::vec2 uv{};

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		bool operator==(const Vertex& other) const {
			return position == other.position
				&& color == other.color
				&& normal == other.normal
				&& uv == other.uv;
		}
	};

	struct Builder {
		std::vector<Vertex> vertices{};
		std::vector<U32> indices{};

		void loadModel(const std::string& filePath);
	};

	Model(Device& device, const Builder& builder);
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	static std::unique_ptr<Model> createModelFromFile(Device& device, const std::string& filePath);

	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

 private:

	void createVertexBuffers(const std::vector<Vertex>& vertices);
	void createIndexBuffers(const std::vector<U32>& indices);

	Device& _device;

 	VkBuffer _vertexBuffer;
	VkDeviceMemory _vertexBufferMemory;
	U32 _vertexCount;

	bool _hasIndexBuffer = false;

	VkBuffer _indexBuffer;
	VkDeviceMemory _indexBufferMemory;
	U32 _indexCount;
};

}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_MODEL_HPP_
