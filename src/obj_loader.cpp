#include "obj_loader.hpp"
#include "model.hpp"
#include "entity.hpp"
#include "utils.hpp"

#include <sstream>
#include <iostream>

std::istream& operator>>(std::istream& is, btVector3& v) {
  for (int i = 0; i < 3; ++i) is >> v[i];
  return is;
}

ObjLoader::ObjLoader() {
  reg("mtllib", m_mtllib);
  reg("o", &ObjLoader::object);
  reg("v", m_vertices);
  reg("vn", m_normals);
  //reg("vt", m_uvs);
  reg("usemtl", m_material);
  reg("s", m_smooth);
  reg("f", &ObjLoader::face);
}

void ObjLoader::object(std::istream& line) {
  generateObject();
  line >> m_object;
}

void ObjLoader::index(std::istream& line) {
  Index index;
  line >> index.v; line.get();
  if (line.peek() != '/') line >> index.vt; line.get();
  line >> index.vn;

  unsigned int& idx = m_indexMap[index];
  if (idx == 0) {
    m_uniqueIndices.push_back(index);
    idx = m_uniqueIndices.size();
  }
  m_indices.push_back(idx-1);
}

void ObjLoader::face(std::istream& line) {
  for(int i = 0; i < 3; ++i) {
    std::string e;
    line >> e;
    std::istringstream ss(e);
    ss.exceptions(std::ios_base::badbit);
    index(ss);
  }
}

void ObjLoader::start() {
  m_objects.clear();
  m_root.reset();
}

void ObjLoader::finish() {
  generateObject();
}

void ObjLoader::generateObject() {
  if (m_uniqueIndices.size() > 0) {
    TriMesh* mesh = new TriMesh;
    mesh->m_indices.copyFromArray(m_indices);
    //std::swap(mesh->m_indices.m_size, m_indices.m_size);
    //std::swap(mesh->m_indices.m_capacity, m_indices.m_capacity);
    //std::swap(mesh->m_indices.m_data, m_indices.m_data);

    bool has_uvs = m_uniqueIndices[0].vt > 0,
         has_normals = m_uniqueIndices[0].vn > 0;
    for (int i = 0; i < m_uniqueIndices.size(); ++i) {
      const Index& idx = m_uniqueIndices[i];
      /// @todo check if index is valid
      mesh->m_vertices.push_back(m_vertices[idx.v-1]);
      if (has_normals) mesh->m_normals.push_back(m_normals[idx.vn-1]);
    }

    EntityPtr entity(new Entity(mesh));
    /// @todo material
    std::string name = m_object;
    std::set<std::string> names;
    for (std::list<EntityPtr>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
      names.insert((*it)->name());
    int i = 0;
    while (names.count(name)) name = m_object + Utils::toString(++i);
    entity->setName(name);
    m_objects.push_back(entity);
  }

  m_vertices.clear();
  m_normals.clear();
  m_indices.clear();
  m_uniqueIndices.clear();
  m_indexMap.clear();
  m_object.clear();
}

EntityPtr ObjLoader::entity() {
  if (m_root) return m_root;
  if (m_objects.size() == 1) return m_objects.front();
  m_root.reset(new Entity);
  for (std::list<EntityPtr>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
    m_root->addChild(*it);
  return m_root;
}
