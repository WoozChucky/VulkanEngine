//
// Created by nunol on 12/25/2021.
//

#ifndef THIS_ENGINE_GRAPHICS_TEXTURECACHE_HPP_
#define THIS_ENGINE_GRAPHICS_TEXTURECACHE_HPP_

#include <string>
#include <map>
#include "GLTexture.hpp"

class TextureCache {

 public:

	GLTexture getTexture(std::string texturePath);

 private:
	std::map<std::string, GLTexture> _textureMap;
};

#endif //THIS_ENGINE_GRAPHICS_TEXTURECACHE_HPP_
