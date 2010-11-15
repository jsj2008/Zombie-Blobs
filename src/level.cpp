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

Level::Level() : m_vbo(0) {
  load();
}

void Level::load() {
  assert( m_heightMap.load(std::string("map.tga")) );
  m_material = new Material();
  GLProgram * program = new GLProgram();
  program->addShader("level.vs", Shader::Vertex);
  program->addShader("level.fs", Shader::Fragment);
  m_material->setShader(program);
}


void Level::update(float dt) {
  static bool ok = false;
  if (ok) return;
  ok = true;
  MarchingCubes::triangulateGrid(m_heightMap.m_data, m_heightMap.m_width, m_heightMap.m_height,
                                 m_verts, m_normals);
  Log::info("vertices %d, m_normals %d", m_verts.size(), m_normals.size());
}


void Level::render(RenderContext &r, bool bind_shader) {
  Entity::render(r, bind_shader);

  glColor4f(0.5, 0.4, 0.3, 0.9);
  float s = 1;

  static GLuint m_vbo = 0;
  if (m_vbo == 0) {
    for (int i=0; i < m_verts.size(); ++i) {
      m_verts[i].setW(1);
      m_normals[i] *= -1; // make normals point to interior
      m_normals[i].setW(1);
    }
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(btVector3)*(m_verts.size()+m_normals.size()), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(btVector3)*m_verts.size(), &m_verts[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(btVector3)*m_verts.size(), sizeof(btVector3)*m_verts.size(), &m_normals[0]);
  }

  //glDisable(GL_CULL_FACE);
	// we want the surface inside out
  glFrontFace(GL_CW);

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
