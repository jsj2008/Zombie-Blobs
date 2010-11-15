#include "material.hpp"

#include "shader.hpp"
#include "game.hpp"

Material::Material() : m_shader(0) {
}

void Material::bind(RenderContext& r, Renderable& obj, bool bind_shader) {
  if (bind_shader && m_shader) {
    m_shader->bind();
    m_shader->setUniform("time", Game::gametime);
  }
}
