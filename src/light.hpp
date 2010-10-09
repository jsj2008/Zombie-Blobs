#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "state.hpp"

#include <LinearMath/btVector3.h>

/**
 * OpenGL Light.
 */
class Light {
public:
  enum Type { Spot, Direction };

  Light();

  /// Enables the light
  void activate(State& state);
  /// Disables the light
  void deactivate(State& state);

protected:
  Type m_type;

  /// The id (beginning from 0) of the light. If it's negative, it will
  /// be chosen automatically in activate().
  int m_id;

  /// Light colors
  GLfloat m_ambient[4], m_diffuse[4], m_specular[4];

  /// if m_type == Spot, we control the light with position and the target vectors
  btVector3 m_position, m_target;

  /// With direction lights (m_type == Direction) there is only direction vector
  btVector3 m_direction;

  /// The maximum spread angle of a light source.
  float m_spot_cutoff;
};

#endif // LIGHT_HPP
