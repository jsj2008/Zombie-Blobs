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
  void setBuffer(int buffer, int type) { m_bufferIndex = buffer; m_type = type; }
  void applyBufferState(RenderContext& r);
  void setBufferState(int cap, bool state) { m_bufferState[cap] = state; }

protected:
  unsigned int m_id;
  int m_type, m_active_type;
  int m_width, m_height;
  int m_bufferIndex;
  std::set<unsigned int> m_fbos;

  std::map<int, bool> m_bufferState;
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

  void set(int type, int index, FBOImagePtr buffer);
  int width() const { return m_width; }
  int height() const { return m_height; }

  void bind(RenderContext& r);
  void unbind();

  static int colorAttachments();

private:
  typedef std::map<int, FBOImagePtr> Buffers;
  Buffers m_buffers;
  unsigned int m_id;
  int m_width, m_height;
};


#endif // FBO_HPP
