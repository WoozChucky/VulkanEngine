//
// Created by nunol on 12/24/2021.
//

#include <Engine/GUI/Sprite.hpp>

#include <Engine/Graphics/Vertex.hpp>
#include "Engine/ResourceManager.hpp"

Sprite::Sprite() {
	_vboId = 0;
}

Sprite::~Sprite() {
	if (_vboId != 0) {
		glDeleteBuffers(1, &_vboId);
	}
}

void Sprite::init(R32 x, R32 y, R32 width, R32 height, const std::string& texturePath) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	// texture cache loading here
	_texture = ResourceManager::getTexture(texturePath);

	if (_vboId == 0) {
		glGenBuffers(1, &_vboId);
	}

	Vertex vertexData[6];
	// first triangle
	vertexData[0].setPosition(x + width, y + height);
	vertexData[0].setUV(1.0f, 1.0f);

	vertexData[1].setPosition(x, y + height);
	vertexData[1].setUV(0.0f, 1.0f);

	vertexData[2].setPosition(x, y);
	vertexData[2].setUV(0.0f, 0.0f);

	// seconds triangle+
	vertexData[3].setPosition(x, y);
	vertexData[3].setUV(0.0f, 0.0f);

	vertexData[4].setPosition(x + width, y);
	vertexData[4].setUV(1.0f, 0.0f);

	vertexData[5].setPosition(x + width, y + height);
	vertexData[5].setUV(1.0f, 1.0f);

	for(int i = 0; i < 6; i++) {
		vertexData[i].setColor(255, 0, 255, 255);
	}

	vertexData[1].setColor(0, 0, 255, 255);

	vertexData[4].setColor(0, 255, 0, 255);

	glBindBuffer(GL_ARRAY_BUFFER, _vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::draw() {

	glBindTexture(GL_TEXTURE_2D, _texture.id);

	glBindBuffer(GL_ARRAY_BUFFER, _vboId);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
