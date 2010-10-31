#include "level.hpp"

#include <cassert>
#include <string>

#include "utils.hpp"
#include "forward.hpp"
#include "opengl.hpp"

Level::Level() {
  load();
}

void Level::load() {
  assert( m_heightMap.load(std::string("map.tga")) );
}

void Level::update(float dt) {

}

void Level::render(RenderContext &r, bool bind_shader) {
  glColor4f(0.5, 1, 0.3, 0.9);
  float s = 1;
  for (int i=0; i < m_heightMap.m_height-1; ++i) {
    glBegin(GL_TRIANGLE_STRIP);
    for (int j=0; j < m_heightMap.m_width; ++j) {

      glVertex3f(s*(j-m_heightMap.m_width/2),
                 s*((i+1)-m_heightMap.m_height/2),
                 -10 + 6*(1 - m_heightMap.m_data[(i+1)*m_heightMap.m_width+j]/255.0));
      glVertex3f(s*(j-m_heightMap.m_width/2),
                 s*(i-m_heightMap.m_height/2),
                 -10 + 6*(1 - m_heightMap.m_data[i*m_heightMap.m_width+j]/255.0));
    }
    glEnd();
  }
}
