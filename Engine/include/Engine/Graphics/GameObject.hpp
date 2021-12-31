//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_GAMEOBJECT_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_GAMEOBJECT_HPP_

#include <memory>
#include "Abstractions/DataTypes.hpp"
#include "Model.hpp"

namespace nl {

struct Transform2dComponent {
	glm::vec2 translation{}; // (position offset)
	glm::vec2 scale{1.f, 1.f};
	R32 rotation;

	glm::mat2 mat2() {
		const R32 s = glm::sin(rotation);
		const R32 c = glm::cos(rotation);

		glm::mat2 rotMatrix{{c, s}, {-s, c}};
		glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};

		return rotMatrix * scaleMat;
	}
};

class GameObject {

 public:
	using id_t = U32;

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
	Transform2dComponent transform2d;
 private:
	GameObject(id_t id): _id(id) {}

	id_t _id;
};

}

#endif //GL_ENGINE_INCLUDE_ENGINE_GRAPHICS_GAMEOBJECT_HPP_
