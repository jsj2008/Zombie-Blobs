#include "player.hpp"

Player::Player() : Camera() {
}

void Player::rotate(float x, float y) {
  x /= -50; y /= -50;
  m_center = m_center.rotate(btVector3(1, 0, 0), y).rotate(btVector3(0, 0, 1), x);
}
