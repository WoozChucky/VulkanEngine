//
// Created by nunol on 12/24/2021.
//

#ifndef THIS_ENGINE_INPUT_IOMANAGER_HPP_
#define THIS_ENGINE_INPUT_IOMANAGER_HPP_

#include <string>
#include <vector>
#include "include/Abstractions/DataTypes.hpp"

class IOManager {

 public:

	static bool readFileToBuffer(const std::string& filePath, std::vector<U8>& buffer);

};

#endif //THIS_ENGINE_INPUT_IOMANAGER_HPP_
