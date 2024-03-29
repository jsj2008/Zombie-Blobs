#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "forward.hpp"

#include <LinearMath/btVector3.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>

class Physics {
public:
  Physics();

  void init();
  void update(float dt);
  void addTrimesh(const btAlignedObjectArray<btVector3>& verts,
                  const btAlignedObjectArray<unsigned int>& indices,
                  const std::string& filename);
  void addRigidBody(btRigidBody* body);

private:
  btBroadphaseInterface* m_broadphase;
  btSoftRigidDynamicsWorld* m_world;
};

#endif // PHYSICS_HPP
