#include "level.hpp"

#include <cassert>
#include <cmath>
#include <string>

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
  assert( m_heightMap.load(std::string("map.tga")) );
  m_material = new Material();
  GLProgram * program = new GLProgram();
  program->addShader("level.vs", Shader::Vertex);
  program->addShader("level.fs", Shader::Fragment);
  m_material->setShader(program);

  FILE * fp = fopen("level.data", "rb");
  if (!fp) {
    MarchingCubes::triangulateGrid(m_heightMap.m_data, m_heightMap.m_width, m_heightMap.m_height,
                                   m_verts, m_normals);

    btVector3 mins(10000, 10000, 10000);
    btVector3 maxs(0,0,0);
    // make inside vertices CCW order
    for (int i=0; i < m_verts.size(); i+=3) {
      btVector3 tmp = m_verts[i+2];
      m_verts[i+2] = m_verts[i+1];
      m_verts[i+1] = tmp;
    }

    for (int i=0; i < m_verts.size(); ++i) {
      mins.setX(std::min(mins.x(), m_verts[i].x()));
      mins.setY(std::min(mins.y(), m_verts[i].y()));
      mins.setZ(std::min(mins.z(), m_verts[i].z()));

      maxs.setX(std::max(maxs.x(), m_verts[i].x()));
      maxs.setY(std::max(maxs.y(), m_verts[i].y()));
      maxs.setZ(std::max(maxs.z(), m_verts[i].z()));


      m_verts[i].setW(1);
      m_normals[i] *= -1; // make normals point to interior
      m_normals[i].setW(1);
    }
    Log::info("vertices %d, m_normals %d", m_verts.size(), m_normals.size());
    Log::info("bb (%f,%f,%f) -> (%f, %f, %f)",
              mins.x(), mins.y(), mins.z(),
              maxs.x(), maxs.y(), maxs.z()
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

  Game::instance()->physics()->addTrimesh(&m_verts[0], m_verts.size());
}


void Level::update(float dt) {

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

void Level::render(RenderContext &r, bool bind_shader) {
  Entity::render(r, bind_shader);

  glColor4f(0.5, 0.4, 0.3, 0.9);
  float s = 1;

  static GLuint m_vbo = 0;
  if (m_vbo == 0) {

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(btVector3)*(m_verts.size()+m_normals.size()), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(btVector3)*m_verts.size(), &m_verts[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(btVector3)*m_verts.size(), sizeof(btVector3)*m_verts.size(), &m_normals[0]);
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexPointer(4, GL_FLOAT, 0, 0);
  glNormalPointer(GL_FLOAT, sizeof(btVector3), (char*)0 + sizeof(btVector3)*m_verts.size());

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glDrawArrays(GL_TRIANGLES, 0, m_verts.size());
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

#if 0
  glBegin(GL_TRIANGLES);
  for (int i=0; i< m_verts.size(); ++i) {
    glNormal3fv((GLfloat*)m_normals[i].m_floats); //m_normals[i].x(), m_normals[i].y(), m_normals[i].z());
    glVertex3fv((GLfloat*)m_verts[i].m_floats); //m_verts[i].x(), m_verts[i].y(), m_verts[i].z());
  }
  glEnd();
#endif
	/*
  glBegin(GL_LINES);
  for (int i=0; i< m_verts.size(); ++i) {
    glVertex3f(m_verts[i].x(), m_verts[i].y(), m_verts[i].z());
    btVector3 e = m_verts[i] + m_normals[i];
    glVertex3f(e.x(), e.y(), e.z());
  }
  glEnd();
	*/
  glFrontFace(GL_CCW);
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
