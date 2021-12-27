//
// Created by nunol on 12/25/2021.
//

#ifndef THIS_ENGINE_RESOURCEMANAGER_HPP_
#define THIS_ENGINE_RESOURCEMANAGER_HPP_

#include "Graphics/TextureCache.hpp"
class ResourceManager {

 public:

	static GLTexture getTexture(const std::string& texturePath);

 private:

	static TextureCache _textureCache;

};

#endif //THIS_ENGINE_RESOURCEMANAGER_HPP_
