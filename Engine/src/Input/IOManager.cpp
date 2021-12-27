//
// Created by nunol on 12/24/2021.
//

#include <fstream>
#include "Engine//Input/IOManager.hpp"
#include "Abstractions/Environment.hpp"

bool IOManager::readFileToBuffer(const std::string &filePath, std::vector<U8> &buffer) {

	std::ifstream file(filePath, std::ios::binary);

	if (file.fail()) {
		LOGW(__FUNCTION__, "Failed to open file %s. %s", filePath.c_str(), strerror(errno))
		return false;
	}

	file.seekg(0, std::ios::end);

	S64 fileSize = file.tellg();

	file.seekg(0, std::ios::beg);

	// Reduce the file fileSize by any header bytes that might be present
	fileSize -= file.tellg();

	buffer.resize(fileSize);

	file.read((char *)&(buffer[0]), fileSize);

	return true;
}
