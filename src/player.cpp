#include "player.hpp"

#include "LinearMath/btQuaternion.h"

Player::Player() : Camera(), m_theta(0), m_phi(0) {
}

void Player::rotate(float x, float y) {
  m_theta -= y/50; m_phi -= x/50;
  m_center = m_eye + btVector3(0, 1, 0).rotate(btVector3(1, 0, 0), m_theta).rotate(btVector3(0, 0, 1), m_phi);

  //m_center = m_center.rotate(btVector3(1, 0, 0), y).rotate(btVector3(0, 0, 1), x);
}
