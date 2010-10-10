#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "forward.hpp"

class Material {
public:
  Material();
  void bind(RenderContext& r, Renderable& obj, bool bind_shader);

  GLProgram* shader() { return m_shader; }

private:
  GLProgram* m_shader;
};

#endif // MATERIAL_HPP
