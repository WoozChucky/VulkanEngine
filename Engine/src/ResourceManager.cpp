//
// Created by nunol on 12/25/2021.
//

#include "Engine/ResourceManager.hpp"

GLTexture ResourceManager::getTexture(const std::string &texturePath) {
	return _textureCache.getTexture(texturePath);
}

TextureCache ResourceManager::_textureCache;
