//
// Created by nunol on 12/25/2021.
//

#ifndef THIS_ENGINE_GRAPHICS_PICOPNG_HPP_
#define THIS_ENGINE_GRAPHICS_PICOPNG_HPP_

#include <vector>

extern int decodePNG(std::vector<unsigned char>& out_image,
			  unsigned long& image_width,
			  unsigned long& image_height,
			  const unsigned char* in_png,
			  size_t in_size,
			  bool convert_to_rgba32 = true
);

#endif //THIS_ENGINE_GRAPHICS_PICOPNG_HPP_
