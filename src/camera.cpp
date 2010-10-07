#include "camera.hpp"

Camera::Camera() : m_width(0), m_height(0) {
}

void Camera::resize(int width, int height) {
  m_width = width;
  m_height = height;
}
