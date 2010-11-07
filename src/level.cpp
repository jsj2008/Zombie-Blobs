#include "level.hpp"

#include <cassert>
#include <string>

#include "utils.hpp"
#include "forward.hpp"
#include "opengl.hpp"

#include "marching_cubes.hpp"

Level::Level() {
  load();
}

void Level::load() {
  assert( m_heightMap.load(std::string("map.tga")) );
}

std::vector<btVector3> verts;
std::vector<btVector3> normals;

void Level::update(float dt) {
  static bool ok = false;
  if (ok) return;
  ok = true;
  MarchingCubes::triangulateGrid(m_heightMap.m_data, m_heightMap.m_width, m_heightMap.m_height,
                                 verts, normals);
  Log::info("vertices %d, normals %d", verts.size(), normals.size());
}


void Level::render(RenderContext &r, bool bind_shader) {
  glColor4f(0.5, 1, 0.3, 0.9);  
  float s = 1;

  // disable culling for now
  glDisable(GL_CULL_FACE);
  glBegin(GL_TRIANGLES);
  for (int i=0; i< verts.size(); ++i) {
    glNormal3f(normals[i].x(), normals[i].y(), normals[i].z());
    glVertex3f(verts[i].x(), verts[i].y(), verts[i].z());
  }
  glEnd();
  /*
  for (int i=0; i < m_heightMap.m_height-1; ++i) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int j=0; j < m_heightMap.m_width; ++j) {
      glVertex3f(s*(j-m_heightMap.m_width/2),
                 s*((i+1)-m_heightMap.m_height/2),
                 -50 + 20*(m_heightMap.m_data[(i+1)*m_heightMap.m_width+j]/255.0));
      glVertex3f(s*(j-m_heightMap.m_width/2),
                 s*(i-m_heightMap.m_height/2),
                 -50 + 20*(m_heightMap.m_data[i*m_heightMap.m_width+j]/255.0));
    }
    glEnd();
  }
  */
}
