
set(This Engine)

set(Headers
        # Graphics headers
        include/Engine/Graphics/Window.hpp
        include/Engine/Graphics/Pipeline.hpp
        include/Engine/Graphics/Device.hpp
        include/Engine/Graphics/SwapChain.hpp
        include/Engine/Graphics/Model.hpp
        include/Engine/Graphics/GameObject.hpp
        include/Engine/Graphics/Renderer.hpp
        include/Engine/Graphics/SimpleRenderSystem.hpp
        include/Engine/Graphics/Camera.hpp
        include/Engine/Graphics/Buffer.hpp
        include/Engine/Graphics/FrameInfo.hpp
        # Input headers
        include/Engine/Input/IOManager.hpp
        include/Engine/Input/KeyboardMovementController.hpp
        )

set(Sources
        src/Graphics/Window.cpp
        src/Graphics/Pipeline.cpp
        src/Graphics/Device.cpp
        src/Graphics/SwapChain.cpp
        src/Graphics/Model.cpp
        src/Graphics/GameObject.cpp
        src/Graphics/Renderer.cpp
        src/Graphics/SimpleRenderSystem.cpp
        src/Graphics/Camera.cpp
        src/Graphics/Buffer.cpp

        src/Input/IOManager.cpp
        src/Input/KeyboardMovementController.cpp
        )

include_directories(../Abstractions)

add_library(${This} STATIC ${Sources} ${Headers})

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

target_link_libraries(${This} Abstractions glfw)
