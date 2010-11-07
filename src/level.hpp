#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "entity.hpp"
#include "tga_image.h"
#include "forward.hpp"

#include <vector>

#include <LinearMath/btVector3.h>

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
	unsigned int m_vbo;	
  std::vector<btVector3> m_verts;
  std::vector<btVector3> m_normals;
};


#endif // LEVEL_HPP
