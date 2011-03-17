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

  void setFormat(int format) {
    m_format = format;
  }

  void setup(unsigned int fbo, int width, int height);
  void setTextureParameter(GLenum pname, GLint param) { m_parameters[pname] = param; }

  void bind(int texture, int width, int height);
  void bind(int texture = 0);
  void unbind();

protected:
  virtual void texImage(int width, int height);

  int m_format;
  unsigned int m_id, m_bindedTexture;

  typedef std::map<GLenum, GLint> Parameters;
  Parameters m_parameters;
};

#endif // TEXTURE_HPP
