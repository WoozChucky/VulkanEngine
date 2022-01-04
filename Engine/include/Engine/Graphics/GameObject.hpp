//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_GAMEOBJECT_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_GAMEOBJECT_HPP_

#include <memory>
#include "Abstractions/DataTypes.hpp"
#include "Model.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

namespace nl::gfx {

struct TransformComponent {
	glm::vec3 translation{}; // (position offset)
	glm::vec3 scale{1.f, 1.f, 1.f};
	glm::vec3 rotation;

	// Matrix corresponds to translate * Ry * Rx * Rz * scale transformation
	// Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
	glm::mat4 mat4();
	glm::mat3 normalMatrix();
};

class GameObject {

 public:
	using id_t = U32;
	using Map = std::unordered_map<id_t, GameObject>;

	static GameObject createGameObject() {

		static id_t currentId = 0;
		return GameObject{currentId++};
	}

	GameObject(const GameObject&) = delete;
	GameObject &operator=(const GameObject&) = delete;
	GameObject(GameObject&&) = default;
	GameObject &operator=(GameObject&&) = default;

	id_t getId() const { return _id; };

	std::shared_ptr<Model> model{};
	glm::vec3 color{};
	TransformComponent transform{};
 private:
	GameObject(id_t id): _id(id) {}

	id_t _id;
};

}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_GAMEOBJECT_HPP_
