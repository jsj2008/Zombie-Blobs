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

  /// Returns the uniform name for this texture
  /// @todo how to do this exactly, maybe we should ask from the shader instead of texture?
  std::string name() const { return m_name; }
  void setName(const std::string & name) { m_name = name; }
private:
  unsigned int m_id, m_bindedTexture;
  std::string m_name;
};

#endif // TEXTURE_HPP
