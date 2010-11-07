#include "player.hpp"

#include "LinearMath/btQuaternion.h"

#include <algorithm>

Player::Player() : Camera(), m_theta(0), m_phi(0) {
}

void Player::move(float dx, float dy, float dz)
{
  btVector3 dir = m_eye - m_center;
  btVector3 n = dir.normalized();
  btVector3 move = -n * dy + btVector3(-n.y(), n.x(), 0) * dx + btVector3(0, 0, 1) * dz;
  m_center += move;
  m_eye = m_center + dir;
}

void Player::rotate(float x, float y) {
  m_theta -= y/50; m_phi -= x/50;
  m_theta = std::max(-M_PI_2+1e-5, std::min(M_PI_2-1e-5, (double)m_theta));
  m_center = m_eye + btVector3(0, 1, 0).rotate(btVector3(1, 0, 0), m_theta).rotate(btVector3(0, 0, 1), m_phi);
}
