//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ENGINE_INCLUDE_ENGINE_INPUT_KEYBOARDMOVEMENTCONTROLLER_HPP_
#define GL_ENGINE_INCLUDE_ENGINE_INPUT_KEYBOARDMOVEMENTCONTROLLER_HPP_

#include "Abstractions/DataTypes.hpp"
#include "Engine/Graphics/GameObject.hpp"
#include "Engine/Graphics/Window.hpp"

namespace nl::io {

class KeyboardMovementController {

 public:

	struct KeyMapping {
		S32 moveLeft = GLFW_KEY_A;
		S32 moveRight = GLFW_KEY_D;
		S32 moveForward = GLFW_KEY_W;
		S32 moveBackward = GLFW_KEY_S;
		S32 moveUp = GLFW_KEY_E;
		S32 moveDown = GLFW_KEY_Q;
		S32 lookLeft = GLFW_KEY_LEFT;
		S32 lookRight = GLFW_KEY_RIGHT;
		S32 lookUp = GLFW_KEY_UP;
		S32 lookDown = GLFW_KEY_DOWN;
	};

	void moveInPlaneXZ(GLFWwindow* window, R32 dt, gfx::GameObject& gameObject);

	KeyMapping keys{};
	R32 moveSpeed{3.f};
	R32 lookSpeed{1.5f};

 private:

};

}

#endif //GL_ENGINE_INCLUDE_ENGINE_INPUT_KEYBOARDMOVEMENTCONTROLLER_HPP_
