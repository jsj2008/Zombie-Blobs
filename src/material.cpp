#include "material.hpp"

#include "shader.hpp"
#include "game.hpp"

Material::Material(GLProgram* shader) : m_shader(shader) {
}

void Material::bind(RenderContext& r, Renderable& obj, bool bind_shader) {
  if (bind_shader && m_shader) {
    glCheck("material bind");
    m_shader->bind(r);
    m_shader->setUniform(r, "time", Game::gametime);
  }
}
