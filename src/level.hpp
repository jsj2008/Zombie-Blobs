#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "entity.hpp"
#include "tga_image.h"
#include "forward.hpp"

#include <vector>

#include <LinearMath/btVector3.h>
#include <LinearMath/btAlignedObjectArray.h>

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

  const btAlignedObjectArray<Enemy*>& getEnemies() { return m_blobs; }
  TexturePtr mapTexture() { return m_map; }
private:
  TGAImage m_heightMap;
  unsigned int m_vbo;

  btAlignedObjectArray<btVector3> m_verts;
  btAlignedObjectArray<unsigned int> m_indices;
  btAlignedObjectArray<btVector3> m_normals;
  btAlignedObjectArray<btVector3> m_blobSpawns;
  btAlignedObjectArray<Enemy*> m_blobs;
  typedef std::set<std::pair<Enemy*, Enemy*> > EnemyGraph;
  EnemyGraph m_neighbours;
  btVector3 m_bb[2]; // (min, max)

  TexturePtr m_map;

  unsigned int m_permtex, m_gradtex;

  Material * m_blobMaterial;
};


#endif // LEVEL_HPP
