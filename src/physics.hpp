#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include "forward.hpp"

class Physics {
public:
  Physics();

  void init();
  void update(float dt);

private:
  btBroadphaseInterface* m_broadphase;
  btSoftRigidDynamicsWorld* m_world;
};

#endif // PHYSICS_HPP
