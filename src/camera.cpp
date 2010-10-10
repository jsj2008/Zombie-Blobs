#include "camera.hpp"
#include "opengl.hpp"

#include <cassert>

Camera::Camera() : m_type(Perspective), m_fov(45), m_near(0.1f), m_far(1000.0f) {}

void Camera::prepare(int width, int height) {
  glCheck("Camera::prepare");
  glViewport(0, 0, width, height);
  m_width = width;
  m_height = height;
  if (m_type == Perspective) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_fov, (GLfloat)width/height, m_near, m_far);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
//    gluLookAt(m_position.x(), m_position.y(), m_position.z(),
//              m_target.x(), m_target.y(), m_target.z(),
//              m_up.x(), m_up.y(), m_up.z());
  } else if (m_type == Rect) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, m_near, m_far);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
  }
}

void Camera::setRect(float near_, float far_) {
  m_type = Rect;
  m_near = near_;
  m_far = far_;
}
