//
// Created by nunol on 1/4/2022.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_FRAMEINFO_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_FRAMEINFO_HPP_

#include <vulkan/vulkan.h>
#include "Abstractions/DataTypes.hpp"
#include "Camera.hpp"

namespace nl::gfx {

struct FrameInfo {
	S32 frameIndex;
	R32 frameTime;
	VkCommandBuffer buffer;
	Camera& camera;
};

}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_FRAMEINFO_HPP_
