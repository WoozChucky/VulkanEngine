//
// Created by nunol on 12/31/2021.
//

#include <Engine/Graphics/Model.hpp>

nl::Model::Model(Device &device, const std::vector<Vertex> &vertices): _device(device) {
	createVertexBuffers(vertices);
}

nl::Model::~Model() {
	vkDestroyBuffer(_device.device(), _vertexBuffer, nullptr);
	vkFreeMemory(_device.device(), _vertexBufferMemory, nullptr);
}

void nl::Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
	_vertexCount = static_cast<U32>(vertices.size());
	assert(_vertexCount >= 3);

	VkDeviceSize  bufferSize = sizeof(Vertex) * _vertexCount;

	_device.createBuffer(bufferSize,
						 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
						 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
						 _vertexBuffer,
						 _vertexBufferMemory);

	void* data;

	vkMapMemory(_device.device(), _vertexBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(_device.device(), _vertexBufferMemory);
}

void nl::Model::bind(VkCommandBuffer commandBuffer) {
	VkBuffer buffers[] = {_vertexBuffer};
	VkDeviceSize offsets[]  = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void nl::Model::draw(VkCommandBuffer commandBuffer) {
	vkCmdDraw(commandBuffer, _vertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription> nl::Model::Vertex::getBindingDescriptions() {

	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> nl::Model::Vertex::getAttributeDescriptions() {

	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);

	return attributeDescriptions;
}
