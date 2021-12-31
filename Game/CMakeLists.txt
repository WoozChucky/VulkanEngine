set(This Game)

set(Headers
        # src/MainGame.hpp
        )

set(Sources
        # src/MainGame.cpp
        src/main.cpp
        src/Application.cpp
        src/Application.hpp)


include_directories(../Abstractions)
include_directories(../Engine)

find_library(LIB_VULKAN vulkan-1.lib PATHS C:\\VulkanSDK\\1.2.198.1\\Lib)

add_executable(${This} ${Sources} ${Headers})

set_target_properties(${This} PROPERTIES
        FOLDER Applications
        )

target_link_libraries(${This} Abstractions Engine ${LIB_VULKAN})
