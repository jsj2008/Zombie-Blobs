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

  btVector3 playerSpawnPoint();

  btVector3 blobSpawnPoint();

  void spawnNewEnemy();

  btVector3* aabb() { return m_bb; }
  void load();
  // winning condition
  // random seed
private:
  TGAImage m_heightMap;
	unsigned int m_vbo;	
  std::vector<btVector3> m_verts;
  std::vector<btVector3> m_normals;
  std::vector<btVector3> m_blobSpawns;
  std::vector<Enemy*> m_blobs;
  typedef std::set<std::pair<Enemy*, Enemy*> > EnemyGraph;
  EnemyGraph m_neighbours;
  btVector3 m_bb[2]; // (min, max)

  Material * m_blobMaterial;
};


#endif // LEVEL_HPP
