#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "camera.hpp"

class Player : public Camera {
public:
  Player();
  void move(float dx, float dy);
  void rotate(float x, float y);
private:
  float m_theta, m_phi;
};

#endif // PLAYER_HPP
