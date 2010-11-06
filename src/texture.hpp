#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "forward.hpp"
#include "fbo.hpp"

#include <string>

class Texture : public FBOImage {
public:
  Texture();
  virtual ~Texture();

  void setup(unsigned int fbo, int width, int height);

  void bind(int texture = 0);
  void unbind();

private:
  unsigned int m_id, m_bindedTexture;
};

#endif // TEXTURE_HPP
