//
// Created by nunol on 12/25/2021.
//

#include "TextureCache.hpp"
#include "ImageLoader.hpp"
#include "Core/Env/Environment.hpp"

GLTexture TextureCache::getTexture(std::string texturePath) {

	auto iterator = _textureMap.find(texturePath);

	if (iterator == _textureMap.end()) {
		GLTexture newTexture = ImageLoader::loadPNG(texturePath);

		_textureMap.insert(std::make_pair(texturePath, newTexture));

		LOGD(__FUNCTION__, "Loaded texture %s into cache", texturePath.c_str())
		return newTexture;
	}

	LOGD(__FUNCTION__, "Loaded texture %s from cache", texturePath.c_str())

	return iterator->second;
}
