#include "light.hpp"

Light::Light() : m_type(Direction), m_id(-1), m_direction(0, 0, 1), m_spot_cutoff(180) {
  m_ambient[0]  = 0;    m_ambient[1]  = 0;    m_ambient[2]  = 0;    m_diffuse[3]  = 1.0f;
  m_diffuse[0]  = 1.0f; m_diffuse[1]  = 1.0f; m_diffuse[2]  = 1.0f; m_ambient[3]  = 1.0f;
  m_specular[0] = 1.0f; m_specular[1] = 1.0f; m_specular[2] = 1.0f; m_specular[3] = 1.0f;
}

void Light::activate(State& state) {
  if (m_id < 0) m_id = state.nextFreeLight();
  state.setLight(m_id, true);

  glRun(glLightfv(GL_LIGHT0+m_id, GL_AMBIENT, m_ambient));
  glRun(glLightfv(GL_LIGHT0+m_id, GL_DIFFUSE, m_diffuse));
  glRun(glLightfv(GL_LIGHT0+m_id, GL_SPECULAR, m_specular));

  if (m_type == Spot) {
    m_position.setW(1.0f);
    glRun(glLightfv(GL_LIGHT0+m_id, GL_POSITION, m_position));

    btVector3 n = m_target - m_position;
    n.normalize();

    glRun(glLightfv(GL_LIGHT0+m_id, GL_SPOT_DIRECTION, n));
    glRun(glLightf(GL_LIGHT0+m_id, GL_SPOT_CUTOFF, m_spot_cutoff));
  } else {
    m_position.setW(0.0f);
    glRun(glLightfv(GL_LIGHT0+m_id, GL_POSITION, m_position));
  }
}

void Light::deactivate(State& state) {
  state.setLight(m_id, false);
  m_id = -1;
}
