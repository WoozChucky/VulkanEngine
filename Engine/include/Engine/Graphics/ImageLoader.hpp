//
// Created by nunol on 12/25/2021.
//

#ifndef THIS_ENGINE_GRAPHICS_IMAGELOADER_HPP_
#define THIS_ENGINE_GRAPHICS_IMAGELOADER_HPP_

#include <string>
#include "GLTexture.hpp"

class ImageLoader {

 public:

	static GLTexture loadPNG(const std::string& filePath);

};

#endif //THIS_ENGINE_GRAPHICS_IMAGELOADER_HPP_
