#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "entity.hpp"
#include "tga_image.h"

class Level : public Entity {
public:
  Level();

  virtual void update(float dt);
  virtual void render(RenderContext &r, bool bind_shader=true);

  void load();
  // winning condition
  // random seed
private:
  TGAImage m_heightMap;
};


#endif // LEVEL_HPP
