//
// Created by nunol on 12/24/2021.
//

#ifndef THIS_ENGINE_GUI_SPRITE_HPP_
#define THIS_ENGINE_GUI_SPRITE_HPP_

#include <string>
#include "GL/glew.h"
#include "Abstractions/DataTypes.hpp"
#include "Engine/Graphics/GLTexture.hpp"

class Sprite {

 public:
	Sprite();
	~Sprite();

	void init(R32 x, R32 y, R32 width, R32 height, const std::string& texturePath);
	void draw();

 private:

	R32 _x;
	R32 _y;
	R32 _width;
	R32 _height;
	U32 _vboId;
	GLTexture _texture;

};

#endif //THIS_ENGINE_GUI_SPRITE_HPP_
