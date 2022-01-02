//
// Created by nunol on 12/320/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_RENDERER_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_RENDERER_HPP_

#include "Engine/Graphics/Window.hpp"
#include "Engine/Graphics/Device.hpp"
#include "Engine/Graphics/SwapChain.hpp"

#include <memory>
#include <cassert>

namespace nl::gfx {

class Renderer {

 public:

	Renderer(Window& window, Device& device);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	VkRenderPass getSwapChainRenderPass() const { return _swapChain->getRenderPass(); }
	R32 getAspectRation() const { return _swapChain->extentAspectRatio(); }
	bool  isFrameInProgress() const { return _frameStarted; }

	VkCommandBuffer getCurrentcommandBuffer() const {
		assert(_frameStarted);
		return _commandBuffers[_currentFrameIndex];
	}

	S32 getFrameIndex() const {
		assert(_frameStarted);
		return _currentFrameIndex;
	}

	VkCommandBuffer beginFrame();
	void endFrame();
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);

 private:
	void createCommandBuffers();
	void freeCommandBuffers();
	void recreateSwapChain();

	Window& _window;
	Device& _device;
	std::unique_ptr<SwapChain> _swapChain;
	std::vector<VkCommandBuffer> _commandBuffers;
	bool _frameStarted{false};
	U32 _currentImageIndex;
	S32 _currentFrameIndex{0};
};


}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_RENDERER_HPP_
