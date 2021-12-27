//
// Created by nunol on 12/24/2021.
//

#include "GLSLProgram.hpp"
#include "Core/Env/Environment.hpp"
#include <fstream>

GLSLProgram::GLSLProgram(): _programId(0), _vertexShaderId(0), _fragmentShaderId(0), _numAttributes(0) {}

GLSLProgram::~GLSLProgram() {

}

void GLSLProgram::compileShaders(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename) {

	_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderId == 0) {
		LOGE(__FUNCTION__, "Failed to create vertex shader for %s", vertexShaderFilename.c_str())
		Environment::destroy(-1);
	}

	_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderId == 0) {
		LOGE(__FUNCTION__, "Failed to create fragment shader for %s", fragmentShaderFilename.c_str())
		Environment::destroy(-1);
	}

	this->compileShader(vertexShaderFilename, _vertexShaderId);

	this->compileShader(fragmentShaderFilename, _fragmentShaderId);
}

void GLSLProgram::addAttribute(const std::string &attributeName) {
	glBindAttribLocation(_programId, _numAttributes++, attributeName.c_str());
}

S32 GLSLProgram::getUniformLocation(const std::string &uniformName) {
	S32 location = glGetUniformLocation(_programId, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		LOGW(__FUNCTION__, "Failed to get uniform location '%s' from shader", uniformName.c_str())
		Environment::destroy(-1);
	}
	return location;
}

void GLSLProgram::linkShaders() {

	_programId = glCreateProgram();

	glAttachShader(_programId, _vertexShaderId);
	glAttachShader(_programId, _fragmentShaderId);

	glLinkProgram(_programId);

	S32 isLinked;

	glGetProgramiv(_programId, GL_LINK_STATUS, &isLinked);

	if (isLinked == GL_FALSE) {
		S32 maxLength;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programId, maxLength, &maxLength, &errorLog[0]);

		glDeleteProgram(_programId);
		glDeleteShader(_vertexShaderId);
		glDeleteShader(_fragmentShaderId);

		LOGW(__FUNCTION__, "Failed to link shaders. Details: %s", &(errorLog[0]))
		Environment::destroy(-1);
	}

	glDetachShader(_programId, _vertexShaderId);
	glDetachShader(_programId, _fragmentShaderId);
	glDeleteShader(_vertexShaderId);
	glDeleteShader(_fragmentShaderId);
}

void GLSLProgram::compileShader(const std::string &shaderFilename, U32 &shaderId) {

	std::ifstream shaderFile(shaderFilename);
	if (shaderFile.fail()) {
		LOGE(__FUNCTION__, "Failed to open file %s", shaderFilename.c_str())
		Environment::destroy(-1);
	}

	std::string fileContents;
	std::string line;

	while (std::getline(shaderFile, line)) {
		fileContents += line + "\n";
	}

	shaderFile.close();

	const char * contentsPtr = fileContents.c_str();
	glShaderSource(shaderId, 1, &contentsPtr, nullptr);

	glCompileShader(shaderId);

	S32 success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		S32 maxLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &errorLog[0]);

		glDeleteShader(shaderId);

		LOGW(__FUNCTION__, "Failed to compile shader %s. Details: %s", shaderFilename.c_str(), &(errorLog[0]))
		Environment::destroy(-1);
	}

	LOGD(__FUNCTION__, "Shader %s compiled with success", shaderFilename.c_str());
}

void GLSLProgram::use() {
	glUseProgram(_programId);
	for(auto i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse() {
	glUseProgram(0);
	for(auto i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}
