#include "level.hpp"
#include "math.hpp"

#include <cassert>
#include <string>

#include "model.hpp"
#include "utils.hpp"
#include "forward.hpp"
#include "opengl.hpp"
#include "material.hpp"
#include "marching_cubes.hpp"
#include "shader.hpp"
#include "game.hpp"
#include "physics.hpp"

Level::Level() : m_vbo(0) {
}

void Level::load() {
  if (m_verts.size())
    return;
  assert( m_heightMap.load(std::string("map.tga")) );
  m_material = new Material();
  GLProgram * program = new GLProgram();
  program->addShader("level.vs", Shader::Vertex);
  program->addShader("level.fs", Shader::Fragment);
  m_material->setShader(program);

#if 1
  m_blobMaterial = new Material();
  GLProgram * blobProgram = new GLProgram();
  blobProgram->addShader("blob.vs", Shader::Vertex);
  blobProgram->addShader("blob.fs", Shader::Fragment);
  m_blobMaterial->setShader(blobProgram);
#else
  m_blobMaterial = 0;
#endif
  FILE * fp = fopen("level.data", "rb");
  if (!fp) {
    MarchingCubes::triangulateGrid(m_heightMap.m_data, m_heightMap.m_width, m_heightMap.m_height,
                                   m_verts, m_normals);

    m_bb[0] = m_bb[1] = m_verts[0];
    // make inside vertices CCW order
    for (size_t i=0; i < m_verts.size(); i+=3) {
      btVector3 tmp = m_verts[i+2];
      m_verts[i+2] = m_verts[i+1];
      m_verts[i+1] = tmp;
    }

    for (size_t i=0; i < m_verts.size(); ++i) {
      m_bb[0].setX(std::min(m_bb[0].x(), m_verts[i].x()));
      m_bb[0].setY(std::min(m_bb[0].y(), m_verts[i].y()));
      m_bb[0].setZ(std::min(m_bb[0].z(), m_verts[i].z()));

      m_bb[1].setX(std::max(m_bb[1].x(), m_verts[i].x()));
      m_bb[1].setY(std::max(m_bb[1].y(), m_verts[i].y()));
      m_bb[1].setZ(std::max(m_bb[1].z(), m_verts[i].z()));


      m_verts[i].setW(1);
      m_normals[i] *= -1; // make normals point to interior
      m_normals[i].setW(1);
    }
    Log::info("vertices %d, m_normals %d", m_verts.size(), m_normals.size());
    Log::info("bb (%f,%f,%f) -> (%f, %f, %f)",
              m_bb[0].x(), m_bb[0].y(), m_bb[0].z(),
              m_bb[1].x(), m_bb[1].y(), m_bb[1].z()
              );

    fp = fopen("level.data", "w+");
    int count = m_verts.size();
    fwrite(&count, sizeof(int), 1, fp);
    fwrite(&m_verts[0], sizeof(btVector3), count, fp);
    fwrite(&m_normals[0], sizeof(btVector3), count, fp);
    fclose(fp);
  } else {
    Log::info("Loading precomputed level from level.data");
    int count;
    fread(&count, sizeof(int), 1, fp);
    m_verts.resize(count); m_normals.resize(count);
    fread(&m_verts[0], sizeof(btVector3), count, fp);
    fread(&m_normals[0], sizeof(btVector3), count, fp);
    fclose(fp);

  }
  Log::info("Adding level mesh to physics engine");
  Game::instance()->physics()->addTrimesh(&m_verts[0], m_verts.size());
  Log::info("Done");

  for (int y=0; y < m_heightMap.m_height; ++y) {
    for (int x=0; x < m_heightMap.m_width; ++x) {
      if (m_heightMap.get(x, y) == 0)
        m_blobSpawns.push_back(btVector3(x, y, 10));
    }
  }
  Log::info("%d blob spawn points", m_blobSpawns.size());
}


void Level::update(float dt) {
  Entity::update(dt);

  m_neighbours.clear();

  for (int i=0; i < m_blobs.size(); ++i) {
    btVector3 p1 = m_blobs[i]->getModel()->getCollisionObject()->getWorldTransform().getOrigin();
    btSphereShape * sp1 = dynamic_cast<btSphereShape*>(m_blobs[i]->getModel()->getCollisionObject()->getCollisionShape());
    float r1 = sp1->getRadius();

    for (int j=i+1; j < m_blobs.size(); ++j) {
      btSphereShape * sp2 = dynamic_cast<btSphereShape*>(m_blobs[i]->getModel()->getCollisionObject()->getCollisionShape());
      float r2 = sp2->getRadius();
      btVector3 p2 = m_blobs[j]->getModel()->getCollisionObject()->getWorldTransform().getOrigin();
      if ((p2-p1).length() < 2.0f + r1 + r2) {
        Enemy * e1 = m_blobs[i];
        Enemy * e2 = m_blobs[j];
        m_neighbours.insert(std::make_pair(std::min(e1, e2), std::max(e1, e2)));
      }
    }
  }
}

// just calculate some seemingly nice point in the middle
btVector3 Level::playerSpawnPoint() {
  btVector3 middle(m_heightMap.m_width/2, m_heightMap.m_height/2, 15);
  uint8_t minim = m_heightMap.get(middle.x(), middle.y());
  btVector3 pos = middle;

  const int Rad = std::min(32, std::min(m_heightMap.m_width, m_heightMap.m_height)/2 - 2);
  for (int dx=-Rad; dx <= Rad; ++dx) {
    for (int dy=-Rad; dy <= Rad; ++dy) {
      btVector3 myPos(middle.x()+dx, middle.y()+dy, 15);
      uint8_t t = m_heightMap.get(myPos.x(), myPos.y());
      if (t < minim) {
        t = minim;
        pos = myPos;
      }
    }
  }
  return pos;
}

btVector3 Level::blobSpawnPoint() {
  return m_blobSpawns[rand() % m_blobSpawns.size()];
}

void Level::spawnNewEnemy() {
  Enemy * e = new Enemy();
  btVector3 pos = blobSpawnPoint();

  e->setMaterial(m_blobMaterial);

  assert ( e->load("blob.obj") );
  btCollisionObject * col =  e->getModel()->getCollisionObject();
  btTransform& trans = col->getWorldTransform();
  trans.setOrigin(pos);

  addChild(RenderablePtr(e));
  m_blobs.push_back(e);
  //gameScene->root()->addChild(RenderablePtr(e));
}


void Level::render(RenderContext &r, bool bind_shader) {
  Entity::render(r, bind_shader);

  float s = 1;

  static GLuint m_vbo = 0;
  if (m_vbo == 0) {

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(btVector3)*(m_verts.size()+m_normals.size()), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(btVector3)*m_verts.size(), &m_verts[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(btVector3)*m_verts.size(), sizeof(btVector3)*m_verts.size(), &m_normals[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  glColor4f(0.9, 0.4, 0.3, 1.0);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexPointer(4, GL_FLOAT, 0, 0);
  glNormalPointer(GL_FLOAT, sizeof(btVector3), (char*)0 + sizeof(btVector3)*m_verts.size());

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  glColor4f(0, 1, 0, 1.0);
  for (EnemyGraph::iterator it = m_neighbours.begin(); it != m_neighbours.end(); ++it) {
    glBegin(GL_QUAD_STRIP);

    btVector3 f = it->first->getModel()->getCollisionObject()->getWorldTransform().getOrigin();
    btVector3 t = it->second->getModel()->getCollisionObject()->getWorldTransform().getOrigin();

    btSphereShape * sp1 = dynamic_cast<btSphereShape*>(it->first->getModel()->getCollisionObject()->getCollisionShape());
    btSphereShape * sp2 = dynamic_cast<btSphereShape*>(it->second->getModel()->getCollisionObject()->getCollisionShape());
    btScalar r1=sp1->getRadius(), r2=sp2->getRadius();

    float coeff = 1 - ((t-f).length() - r1 - r2)/2.0f;
    coeff *= 0.8f;
    r1 *= coeff; r2 *= coeff;

    btVector3 dir1 = t-f;
    btVector3 perp = dir1.rotate(btVector3(0, 0, 1.0f), M_PI/2.0f);
    perp.normalize();

    btVector3 up = perp.cross(dir1).normalized();
    btVector3 norms[8];

    norms[0] = perp*r1;
    norms[1] = perp*r2;
    norms[2] = up*r1;
    norms[3] = up*r2;
    norms[4] = -perp*r1;
    norms[5] = -perp*r2;
    norms[6] = -up*r1;
    norms[7] = -up*r2;

    for (int i=0; i < 10; i+=2) {
      btVector3 norm = norms[i % 8].normalized();
      glNormal3fv(norm);
      glVertex3fv(f + norms[i % 8]);
      glNormal3fv(norm);
      glVertex3fv(t + norms[(i+1) % 8]);
    }

    glEnd();
  }
}
