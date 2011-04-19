#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "forward.hpp"

class Material {
public:
  Material(GLProgram* prog = 0);
  void bind(RenderContext& r, Renderable& obj, bool bind_shader);

  GLProgram* shader() { return m_shader; }
  void setShader(GLProgram * shader) { m_shader = shader; }
private:
  GLProgram* m_shader;
};

#endif // MATERIAL_HPP
