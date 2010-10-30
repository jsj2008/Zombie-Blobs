#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "tga_image.h"

class Level {
public:
  Level();

  void load();
  // winning condition
  // random seed
private:
  TGAImage m_heightMap;
};


#endif // LEVEL_HPP
