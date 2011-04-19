#include "model.hpp"
#include "opengl.hpp"
#include "utils.hpp"

Model::Model(btCollisionObject * col) :
    m_collision(col) {
}

void Model::render() {}

TriMesh::TriMesh() : m_vbo() {}

void TriMesh::render() {
  glColor4f(1, 1, 1, 1);
  int vsize = sizeof(m_vertices[0])*m_vertices.size(),
     nsize = sizeof(m_normals[0])*m_normals.size(),
     isize = sizeof(m_indices[0])*m_indices.size();

  if (m_vbo[0] == 0) {
    glGenBuffers(2, m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vsize+nsize, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vsize, &m_vertices[0]);
    if (nsize) glBufferSubData(GL_ARRAY_BUFFER, vsize, nsize, &m_normals[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, &m_indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[1]);
  glVertexPointer(3, GL_FLOAT, sizeof(m_vertices[0]), 0);
  if (nsize) glNormalPointer(GL_FLOAT, sizeof(m_normals[0]), (char*)0 + vsize);

  glEnableClientState(GL_VERTEX_ARRAY);
  if (nsize) glEnableClientState(GL_NORMAL_ARRAY);
  glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
  glDisableClientState(GL_VERTEX_ARRAY);
  if (nsize) glDisableClientState(GL_NORMAL_ARRAY);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

