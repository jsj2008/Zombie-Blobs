#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "camera.hpp"

class Player : public Camera {
public:
  Player();
  void rotate(float x, float y);
};

#endif // PLAYER_HPP
