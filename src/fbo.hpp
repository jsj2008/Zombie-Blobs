#ifndef FBO_HPP
#define FBO_HPP

#include "forward.hpp"

#include <map>
#include <set>

class FBOImage {
public:
  FBOImage();
  virtual ~FBOImage() {}

  virtual void setup(unsigned int fbo, int width, int height) = 0;
  /// type is GL_DEPTH_ATTACHMENT etc..
  void setType(int type) { m_type = type; }

protected:
  unsigned int m_id;
  int m_type, m_active_type;
  int m_width, m_height;
  std::set<unsigned int> m_fbos;
};

class RenderBuffer : public FBOImage {
public:
  virtual ~RenderBuffer();

  void setup(unsigned int fbo, int width, int height);

private:
  void bind();
  void unbind();
};

class FrameBufferObject {
public:
  FrameBufferObject();
  virtual ~FrameBufferObject();

  void resize(int width, int height);

  void set(int type, FBOImagePtr buffer);
  int width() const { return m_width; }
  int height() const { return m_height; }

  void bind();
  void unbind();

  static int colorAttachments();

private:
  typedef std::map<int, FBOImagePtr> Buffers;
  Buffers m_buffers;
  unsigned int m_id;
  int m_width, m_height;
};


#endif // FBO_HPP
