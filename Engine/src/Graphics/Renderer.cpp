//
// Created by nunol on 12/31/2021.
//

#include "Engine/Graphics/Renderer.hpp"

#include <stdexcept>
#include <array>

using namespace nl::gfx;

Renderer::Renderer(Window &window, Device &device) : _window{window}, _device{device} {
	recreateSwapChain();
	createCommandBuffers();
}

Renderer::~Renderer() {
	freeCommandBuffers();
}

void Renderer::recreateSwapChain() {
	auto extent = _window.getExtent();
	while (extent.width == 0 || extent.height == 0) {
		extent = _window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(_device.device());

	if (_swapChain == nullptr) {
		_swapChain = std::make_unique<SwapChain>(_device, extent);
	} else {
		std::shared_ptr<SwapChain> oldSwapChain = std::move(_swapChain);

		_swapChain = std::make_unique<SwapChain>(_device, extent, oldSwapChain);

		if (!oldSwapChain->compareSwapFormats(*_swapChain.get())) {
			throw std::runtime_error("SwapChain image(or depth) format has changed");
		}
	}
}

void Renderer::createCommandBuffers() {

	_commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = _device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<U32>(_commandBuffers.size());

	if (vkAllocateCommandBuffers(_device.device(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers");
	}
}

void Renderer::freeCommandBuffers() {
	vkFreeCommandBuffers(
		_device.device(),
		_device.getCommandPool(),
		static_cast<U32>(_commandBuffers.size()),
		_commandBuffers.data()
	);
	_commandBuffers.clear();
}

VkCommandBuffer Renderer::beginFrame() {
	assert(!_frameStarted);

	auto result = _swapChain->acquireNextImage(&_currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return nullptr;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to acquire swap chain image");
	}

	_frameStarted = true;

	auto commandBuffer = getCurrentcommandBuffer();

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers");
	}

	return commandBuffer;
}

void Renderer::endFrame() {
	assert(_frameStarted);

	auto commandBuffer = getCurrentcommandBuffer();

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to record command buffers");
	}

	auto result = _swapChain->submitCommandBuffers(&commandBuffer, &_currentImageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _window.wasWindowResized()) {
		_window.resetWindowResized();
		recreateSwapChain();
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swap chain image");
	}

	_frameStarted = false;
	_currentFrameIndex = (_currentFrameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
}

void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
	assert(_frameStarted);
	assert(commandBuffer == getCurrentcommandBuffer());

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _swapChain->getRenderPass();
	renderPassInfo.framebuffer = _swapChain->getFrameBuffer(_currentImageIndex);

	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = _swapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};

	renderPassInfo.clearValueCount = static_cast<U32>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<R32>(_swapChain->getSwapChainExtent().width);
	viewport.height = static_cast<R32>(_swapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{{0,0}, _swapChain->getSwapChainExtent()};
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
	assert(_frameStarted);
	assert(commandBuffer == getCurrentcommandBuffer());
	vkCmdEndRenderPass(commandBuffer);
}
