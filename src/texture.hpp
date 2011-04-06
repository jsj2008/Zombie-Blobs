#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "forward.hpp"
#include "fbo.hpp"
#include "opengl.hpp"

#include <string>

class Texture : public FBOImage {
public:
  Texture();
  virtual ~Texture();

  void setInternalFormat(int format) {
    m_internalFormat = format;
  }
  void setDataType(GLenum type) {
    m_dataType = type;
  }
  void setFormat(GLenum format) {
    m_format = format;
  }

  void setup(unsigned int fbo, int width, int height);
  void setTextureParameter(GLenum pname, GLint param) { m_parameters[pname] = param; }

  void bind(int texture, int width, int height);
  void bind(int texture = 0);
  void unbind();
  virtual void* getData() { return 0; }

protected:
  virtual void texImage(int width, int height);

  int m_internalFormat;
  GLenum m_format;
  GLenum m_dataType;
  unsigned int m_id, m_boundTexture;


  typedef std::map<GLenum, GLint> Parameters;
  Parameters m_parameters;
};

class ImageTexture : public Texture {
public:
  ImageTexture()
    : m_data(0), m_owned(false) {

  }

  virtual void* getData() {
    return m_data;
  }

  virtual ~ImageTexture() {
    if (m_owned)
      delete[] m_data;
  }

  void setData(uint8_t* data, bool own_data) {
    if (m_owned)
      delete[] m_data;

    m_data = data;
    m_owned = own_data;
  }

private:
  uint8_t* m_data;
  bool m_owned;
};


#endif // TEXTURE_HPP
