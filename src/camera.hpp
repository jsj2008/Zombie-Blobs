#ifndef CAMERA_HPP
#define CAMERA_HPP

/**
 * OpenGL Camera class, handles perspective and ortho projections.
 */
class Camera {
public:
  enum Type { Perspective, Rect };

  Camera();

  /// Set the camera with screen size width x height
  void prepare(int width, int height);

  /// Sets this camera to use simple orthographic projection for post processing
  void setRect(float near = -1.0f, float far = 1.0f);

protected:
  Type m_type;

  /// Field of view, in degrees, in the y direction (Perspective)
  float m_fov;
  /// Near clipping plane distance
  float m_near;
  /// Far clipping plane distance
  float m_far;
};

#endif // CAMERA_HPP
