#ifndef TGA_IMAGE_H
#define TGA_IMAGE_H

#include "forward.hpp"

#include <stdint.h>
#include <string>

// only 8-bit grayscale supported
class TGAImage
{
public:
    TGAImage();
    bool load(const std::string & from);
    uint8_t get(int x, int y);
    uint8_t * m_data;
    int m_width;
    int m_height;

};

#endif // TGA_IMAGE_H
