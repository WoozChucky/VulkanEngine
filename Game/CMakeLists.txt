set(This Game)

set(Headers
        src/MainGame.hpp
        )

set(Sources
        src/MainGame.cpp
        src/main.cpp
        )

include_directories(../SDL/include)
include_directories(../vendor/glew/include)
include_directories(../Abstractions)
include_directories(../Engine)

add_executable(${This} ${Sources} ${Headers})

set_target_properties(${This} PROPERTIES
        FOLDER Applications
        )

target_link_libraries(${This} Abstractions Engine)
