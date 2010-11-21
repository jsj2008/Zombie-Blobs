#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "camera.hpp"

#include <btBulletCollisionCommon.h>

class Player : public Camera {
public:
  Player();

  void init(btVector3 pos);
  void update();
  void move(float dx, float dy, float dz);
  void rotate(float x, float y);
private:
  float m_theta, m_phi;
  btRigidBody * m_body;
};

#endif // PLAYER_HPP
