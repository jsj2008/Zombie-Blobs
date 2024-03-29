#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <LinearMath/btVector3.h>
#include <LinearMath/btAlignedObjectArray.h>
#include <vectormath/vmInclude.h>

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

  float aspect() const { return float(m_width)/m_height; }
  float fov() const { return m_fov; }
  float neardist() const { return m_near; }
  float fardist() const { return m_far; }
  void setFar(float far_) { m_far = far_; }
  void setNear(float near_) { m_near = near_; }

  void moveTo(const btVector3& pos, bool preserve_look_dir=false) {
    btVector3 diff = m_center - m_eye;
    m_eye = pos;
    if (!preserve_look_dir)
      m_center = m_eye + diff;
  }

  // Calculates the eight corners of the view frustum in
  // world coordinates
  bool frustumCorners(btAlignedObjectArray<btVector3>& corners);

  /// @todo
  btVector3 pos() const { return m_eye; }
  btVector3 front() const { return m_center - m_eye; }
  btVector3 right() const { return btVector3(1, 0, 0); }
  btVector3 up() const { return m_up; }

protected:
  Type m_type;

  int m_width, m_height;

  /// Field of view, in degrees, in the y direction (Perspective)
  float m_fov;
  /// Near clipping plane distance
  float m_near;
  /// Far clipping plane distance
  float m_far;

  btVector3 m_center;
  btVector3 m_eye;
  btVector3 m_up;
};

#endif // CAMERA_HPP
