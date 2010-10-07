#ifndef CAMERA_HPP
#define CAMERA_HPP

/// Defines the viewport and 3D camera transformation.
class Camera {
public:
  Camera();
  void resize(int width, int height);

  int m_width, m_height;
};

#endif // CAMERA_HPP
