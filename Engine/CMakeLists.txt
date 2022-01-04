
set(This Engine)

file(GLOB_RECURSE Headers include/*.hpp)

file(GLOB_RECURSE Sources src/*.cpp)

include_directories(../Abstractions)

add_library(${This} STATIC ${Sources} ${Headers})

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

target_link_libraries(${This} Abstractions glfw)
