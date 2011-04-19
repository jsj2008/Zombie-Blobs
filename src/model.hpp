#ifndef MODEL_HPP
#define MODEL_HPP

#include <BulletDynamics/btBulletDynamicsCommon.h>

class Model {
public:
  Model(btCollisionObject * col=0);
  virtual ~Model() {}
  btCollisionObject * getCollisionObject()  { return m_collision; }
  void setCollisionObject(btCollisionObject* collision) { m_collision = collision; }

  virtual void render();

protected:
  btCollisionObject * m_collision;
};

class TriMesh : public Model {
public:
  TriMesh();
  virtual ~TriMesh() {}
  virtual void render();

protected:
  btAlignedObjectArray<btVector3> m_vertices;
  btAlignedObjectArray<btVector3> m_normals;
  btAlignedObjectArray<unsigned int> m_indices;

  unsigned int m_vbo[2];

  friend class ObjLoader;
};

#endif // MODEL_HPP
