//
// Created by nunol on 12/31/2021.
//

#ifndef GL_ABSTRACTIONS_INCLUDE_ABSTRACTIONS_UTILS_HPP_
#define GL_ABSTRACTIONS_INCLUDE_ABSTRACTIONS_UTILS_HPP_

#include <cstdio>
#include <functional>

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, const Rest&... rest) {
	seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	(hashCombine(seed, rest), ...);
};

#endif //GL_ABSTRACTIONS_INCLUDE_ABSTRACTIONS_UTILS_HPP_
