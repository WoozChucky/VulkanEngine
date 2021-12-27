//
// Created by nunol on 12/24/2021.
//

#ifndef THIS_ENGINE_GRAPHICS_GLSLPROGRAM_HPP_
#define THIS_ENGINE_GRAPHICS_GLSLPROGRAM_HPP_

#include <string>
#include "Abstractions/DataTypes.hpp"

class GLSLProgram {

 public:
	GLSLProgram();
	~GLSLProgram();

	void compileShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	void addAttribute(const std::string& attributeName);

	S32 getUniformLocation(const std::string& uniformName);

	void linkShaders();

	void use();
	void unuse();

 private:

	U32 _programId;

	U32 _vertexShaderId;
	U32 _fragmentShaderId;

	U32 _numAttributes;

	void compileShader(const std::string& shaderFilename, U32 &shaderId);
};

#endif //THIS_ENGINE_GRAPHICS_GLSLPROGRAM_HPP_
