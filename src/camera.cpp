#include "camera.hpp"
#include "opengl.hpp"
#include "utils.hpp"

#include <vectormath/vmInclude.h>

#include <cassert>


Camera::Camera() : m_type(Perspective), m_fov(45), m_near(0.1f), m_far(1000.0f),
  m_center(0, 1, 0), m_eye(0, 0, 0), m_up(0, 0, 1) {}

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
    gluLookAt(m_eye.x(), m_eye.y(), m_eye.z(),
              m_center.x(), m_center.y(), m_center.z(),
              m_up.x(), m_up.y(), m_up.z());
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

bool Camera::frustumCorners(btAlignedObjectArray<btVector3>& corners) {
  if (m_type != Perspective)
    return false;

  using Vectormath::Aos::Matrix4;
  using Vectormath::Aos::Vector3;
  using Vectormath::Aos::Vector4;
  using Vectormath::Aos::Point3;

  Matrix4 m;

  float aspect = m_width / m_height;
  m = m.perspective(m_fov, aspect, m_near, m_far);
  m *= m.lookAt(Point3(m_eye.x(), m_eye.y(), m_eye.z()),
           Point3(m_center.x(), m_center.y(), m_center.z()),
           Vector3(m_up.x(), m_up.y(), m_up.z()));

  m = Vectormath::Aos::inverse(m);

  Vector4 frustumCorners[8];

  frustumCorners[0] = Vector4(-1.0f, 1.0f, 0.0f, 1.0f);
  frustumCorners[1] = Vector4(1.0f, 1.0f, 0.0f, 1.0f);
  frustumCorners[2] = Vector4(1.0f, -1.0f, 0.0f, 1.0f);
  frustumCorners[3] = Vector4(-1.0f, -1.0f, 0.0f, 1.0f);
  frustumCorners[4] = Vector4(-1.0f, 1.0f, 1.0f, 1.0f);
  frustumCorners[5] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
  frustumCorners[6] = Vector4(1.0f, -1.0f, 1.0f, 1.0f);
  frustumCorners[7] = Vector4(-1.0f, -1.0f, 1.0f, 1.0f);

  for (int i=0; i < 8; ++i) {
    frustumCorners[i] = m*frustumCorners[i];
    frustumCorners[i] /= frustumCorners[i][3];
  }

  corners.resize(8);

  for (int i=0; i < 8; ++i) {
    corners[i].setValue(frustumCorners[i][0],
                        frustumCorners[i][1],
                        frustumCorners[i][2]);
  }

  return true;
}
