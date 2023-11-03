// this code is a prebuilt PNG loader: we may want to look into our own in the future.
// THIS CODE IS NOT OURS
#pragma once

#include <vector>


namespace Jauntlet {
extern int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, std::size_t in_size, bool convert_to_rgba32 = true);
}