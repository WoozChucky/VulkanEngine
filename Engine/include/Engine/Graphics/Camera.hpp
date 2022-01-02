//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_CAMERA_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_CAMERA_HPP_

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "Abstractions/DataTypes.hpp"

namespace nl::gfx {

class Camera{

 public:

	void setOrthographicProjection(R32 left, R32 right, R32 top, R32 bottom, R32 near, R32 far);

	void setPerspectiveProjection(R32 fovY, R32 aspect, R32 near, R32 far);

	void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
	void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
	void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

	inline const glm::mat4& getProjection() const { return _projectionMatrix; }
	inline const glm::mat4& getView() const { return _viewMatrix; }

 private:

	glm::mat4 _projectionMatrix{1.f};
	glm::mat4 _viewMatrix{1.f};

};

}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_CAMERA_HPP_
