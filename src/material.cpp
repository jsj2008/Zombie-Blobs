#include "material.hpp"

#include "shader.hpp"
#include "game.hpp"

Material::Material() : m_shader(0) {
}

void Material::bind(RenderContext& r, Renderable& obj, bool bind_shader) {
  if (bind_shader && m_shader) {
    glCheck("material bind");
    m_shader->bind(r);
    m_shader->setUniform(r, "time", Game::gametime);
  }
}
