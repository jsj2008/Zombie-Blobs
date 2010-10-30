#include "tga_image.h"
#include "resource_manager.hpp"

#include <stdint.h>
#include <cstdio>
#include <cassert>

TGAImage::TGAImage() :
    m_data(0), m_width(0), m_height(0) {
}


bool TGAImage::load(const std::string & from) {
  std::string filename = ResourceManager::find(from);
  uint8_t header[18];
  FILE* fp = fopen(filename.c_str(), "rb");
  if (fread(&header, 1, 18, fp) != 18)
    return false;
  int imageidsize = header[0];
  /* don't care about image id stuff */
  (void)fseek(fp, imageidsize, SEEK_CUR);
  uint16_t width = *(uint16_t*)&header[12];
  uint16_t height = *(uint16_t*)&header[14];

  uint8_t depth = header[16];
  assert(depth == 8);
  m_data = new uint8_t[width * height];
  fread(m_data, width*height, 1, fp);
  printf("%d %d\n", width, height);
  m_width = width;
  m_height = height;
  fclose(fp);
  return true;
}
