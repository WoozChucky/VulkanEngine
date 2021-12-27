//
// Created by nunol on 12/22/2021.
//

#ifndef THIS_FRAMEWORK_DATATYPES_HPP_
#define THIS_FRAMEWORK_DATATYPES_HPP_

#include <functional>

#include "GL/glew.h"

using U8            = uint8_t;
using U16           = uint16_t;
using U32           = uint32_t;
using UL64          = unsigned long;
using U64           = uint64_t;

using S8            = int8_t;
using S16           = int16_t;
using S32           = int32_t;
using S64           = int64_t;

using R32           = float;
using R64           = double;

using BufferSize	= intptr_t;

using AtomicCounter = std::atomic_int_fast64_t;
//using OnKeyCallback 	= std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>;
//using OnMouseCallback 	= std::function<void(GLFWwindow* window, int button, int action, int mods)>;

#endif //THIS_FRAMEWORK_DATATYPES_HPP_
