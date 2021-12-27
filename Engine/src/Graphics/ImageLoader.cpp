//
// Created by nunol on 12/25/2021.
//

#include "Engine/Graphics/ImageLoader.hpp"
#include "Engine/Graphics/PicoPNG.hpp"
#include "Engine/Input/IOManager.hpp"
#include "Abstractions/Environment.hpp"
#include "GL/glew.h"

GLTexture ImageLoader::loadPNG(const std::string &filePath) {

	GLTexture texture = {};

	std::vector<U8> in;
	std::vector<U8> out;
	UL64 width, height;

	if (!IOManager::readFileToBuffer(filePath, in)) {
		LOGE(__FUNCTION__, "Could not read file %s content to buffer", filePath.c_str())
		Environment::destroy(-1);
	}

	auto code = decodePNG(out, width, height, &(in[0]), in.size(), true);
	if (code != 0) {
		LOGE(__FUNCTION__, "DecodePNG failed with error %d", code)
		Environment::destroy(-1);
	}

	glGenTextures(1, &(texture.id));

	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		&(out[0])
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(texture.id);

	glBindTexture(GL_TEXTURE_2D, 0);

	texture.width = width;
	texture.height = height;

	return texture;
}
