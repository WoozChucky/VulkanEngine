cmake_minimum_required(VERSION 3.21)
set(This GL)
project({This})

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#add_subdirectory(Engine)
add_subdirectory(SDL)
add_subdirectory(rabbitmq-c)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

#find_package(SDL2 REQUIRED)
include_directories(SDL/include)
include_directories(rabbitmq-c/include)

#include_directories(vendor/glfw/include)
include_directories(vendor/glew/include)

include_directories(Engine)

#find_library(LIB_GLFW3 glfw3.lib PATHS vendor/glfw/lib-vc2019)
find_library(LIB_GLEW glew32.lib PATHS vendor/glew/win_x64)
#find_library(LIB_SDL2 SDL2.lib PATHS cmake-build-release/SDL)
#find_library(LIB_SDL2Main SDL2main.lib PATHS cmake-build-release/SDL)

# Engine/Core/Display.cpp Engine/Core/Display.hpp

add_executable(GL main.cpp Engine/DataTypes.hpp Engine/Core/Env/Environment.cpp Engine/Core/Env/Environment.hpp Engine/Core/Env/Logger.cpp Engine/Core/Env/Logger.hpp Engine/Core/Env/LogCategory.cpp Engine/Core/Env/LogCategory.hpp Game/MainGame.cpp Game/MainGame.hpp Engine/GUI/Sprite.cpp Engine/GUI/Sprite.hpp Engine/Graphics/GLSLProgram.cpp Engine/Graphics/GLSLProgram.hpp Engine/Graphics/Vertex.hpp Engine/Input/IOManager.cpp Engine/Input/IOManager.hpp Engine/Graphics/PicoPNG.cpp Engine/Graphics/PicoPNG.hpp Engine/Graphics/GLTexture.hpp Engine/Graphics/ImageLoader.cpp Engine/Graphics/ImageLoader.hpp Engine/ResourceManager.cpp Engine/ResourceManager.hpp Engine/Graphics/TextureCache.cpp Engine/Graphics/TextureCache.hpp)

#target_link_libraries(${This} ${LIB_GLFW3} ${LIB_GLEW} ${LIB_SDL2} ${LIB_SDL2Main} SDL2 glu32.lib opengl32.lib)
target_link_libraries(${This} ${LIB_GLEW} rabbitmq SDL2 glu32.lib opengl32.lib)
