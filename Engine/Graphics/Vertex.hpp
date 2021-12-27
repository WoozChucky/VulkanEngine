//
// Created by nunol on 12/24/2021.
//

#ifndef THIS_ENGINE_GRAPHICS_VERTEX_HPP_
#define THIS_ENGINE_GRAPHICS_VERTEX_HPP_

#include "DataTypes.hpp"

struct Position {
	R32 x;
	R32 y;
};

struct Color {
	U8 r;
	U8 g;
	U8 b;
	U8 a;
};

struct UV {
	R32 u;
	R32 v;
};

struct Vertex {

	Position position;
	Color color;
	UV uv;

	void setPosition(R32 x, R32 y) {
		position.x = x;
		position.y = y;
	}

	void setColor(U8 r, U8 g, U8 b, U8 a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}

	void setUV(R32 u, R32 v) {
		uv.u = u;
		uv.v = v;
	}
};

#endif //THIS_ENGINE_GRAPHICS_VERTEX_HPP_
