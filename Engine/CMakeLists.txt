
set(This Engine)

set(Headers
        include/Engine/ResourceManager.hpp
        include/Engine/Input/IOManager.hpp
        include/Engine/GUI/Sprite.hpp
        include/Engine/Graphics/GLSLProgram.hpp
        include/Engine/Graphics/GLTexture.hpp
        include/Engine/Graphics/ImageLoader.hpp
        include/Engine/Graphics/PicoPNG.hpp
        include/Engine/Graphics/TextureCache.hpp
        include/Engine/Graphics/Vertex.hpp
        )

set(Sources
        src/ResourceManager.cpp
        src/Input/IOManager.cpp
        src/GUI/Sprite.cpp
        src/Graphics/GLSLProgram.cpp
        src/Graphics/ImageLoader.cpp
        src/Graphics/PicoPNG.cpp
        src/Graphics/TextureCache.cpp
        )

include_directories(../SDL/include)
include_directories(../vendor/glew/include)
include_directories(../Abstractions)


find_library(LIB_GLEW glew32.lib PATHS ../vendor/glew/win_x64)

add_library(${This} STATIC ${Sources} ${Headers})

target_include_directories(${This} PUBLIC include)

set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

target_link_libraries(${This} ${LIB_GLEW} Abstractions SDL2 glu32.lib opengl32.lib)
