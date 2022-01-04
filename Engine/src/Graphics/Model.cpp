//
// Created by nunol on 12/31/2021.
//

#include <Engine/Graphics/Model.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyObjLoader.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>

#include "Abstractions/Environment.hpp"
#include "Abstractions/Utils.hpp"


namespace std {

template<>
struct hash<nl::gfx::Model::Vertex> {
	size_t operator()(nl::gfx::Model::Vertex const & vertex) const {
		size_t seed = 0;
		hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
		return seed;
	}
};

}

using namespace nl::gfx;

Model::Model(Device &device, const Builder& builder): _device(device) {
	createVertexBuffers(builder.vertices);
	createIndexBuffers(builder.indices);
}

Model::~Model() { }

void Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
	_vertexCount = static_cast<U32>(vertices.size());
	assert(_vertexCount >= 3);

	VkDeviceSize  bufferSize = sizeof(Vertex) * _vertexCount;
	U32 vertexSize = sizeof(Vertex);

	Buffer stagingBuffer {
	 _device,
	 vertexSize,
	 _vertexCount,
	 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	stagingBuffer.map();
	stagingBuffer.writeToBuffer((void*)vertices.data());

	_vertexBuffer = std::make_unique<Buffer>(
		_device,
		vertexSize,
		_vertexCount,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
 	);

	_device.copyBuffer(stagingBuffer.getBuffer(), _vertexBuffer->getBuffer(), bufferSize);
}

void Model::createIndexBuffers(const std::vector<U32> &indices) {
	_indexCount = static_cast<U32>(indices.size());
	_hasIndexBuffer = _indexCount > 0;

	if (!_hasIndexBuffer)
		return;

	VkDeviceSize  bufferSize = sizeof(U32) * _indexCount;
	U32 indexSize = sizeof(U32);

	Buffer stagingBuffer {
		_device,
		indexSize,
		_indexCount,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	stagingBuffer.map();
	stagingBuffer.writeToBuffer((void*)indices.data());

	_indexBuffer = std::make_unique<Buffer>(
		_device,
		indexSize,
		_indexCount,
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	_device.copyBuffer(stagingBuffer.getBuffer(), _indexBuffer->getBuffer(), bufferSize);
}

void Model::draw(VkCommandBuffer commandBuffer) {
	if (_hasIndexBuffer) {
		vkCmdDrawIndexed(commandBuffer, _indexCount, 1, 0, 0, 0);
	} else {
		vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
	}
}

void Model::bind(VkCommandBuffer commandBuffer) {
	VkBuffer buffers[] = {_vertexBuffer->getBuffer()};
	VkDeviceSize offsets[]  = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

	if (_hasIndexBuffer) {
		vkCmdBindIndexBuffer(commandBuffer, _indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

}

std::unique_ptr<Model> Model::createModelFromFile(Device &device, const std::string &filePath) {

	Builder builder{};
	builder.loadModel(filePath);
	LOGD(__FUNCTION__, "Vertex count: %d", builder.vertices.size());

	return std::make_unique<Model>(device, builder);
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {

	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {

	std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

	attributeDescriptions.push_back(
		{0, 0, VK_FORMAT_R32G32B32_SFLOAT, static_cast<U32>(offsetof(Vertex, position))}
	);
	attributeDescriptions.push_back(
		{1, 0, VK_FORMAT_R32G32B32_SFLOAT, static_cast<U32>(offsetof(Vertex, color))}
	);
	attributeDescriptions.push_back(
		{2, 0, VK_FORMAT_R32G32B32_SFLOAT, static_cast<U32>(offsetof(Vertex, normal))}
	);
	attributeDescriptions.push_back(
		{3, 0, VK_FORMAT_R32G32_SFLOAT, static_cast<U32>(offsetof(Vertex, uv))}
	);

	return attributeDescriptions;
}

void Model::Builder::loadModel(const std::string &filePath) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	vertices.clear();
	indices.clear();

	std::unordered_map<Vertex, U32> uniqueVertices{};

	for (const auto& shape : shapes) {
		for (auto& index : shape.mesh.indices) {
			Vertex vertex{};

			if (index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.color = {
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2]
				};

			}

			if (index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};
			}

			if (index.texcoord_index >= 0) {
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<U32>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}

	}

}
