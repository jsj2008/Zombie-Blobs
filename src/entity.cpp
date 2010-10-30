#include "entity.hpp"
#include "material.hpp"

Renderable::Renderable() : m_material(0) {}
Renderable::~Renderable() {}

Entity::Entity() : m_model(0) {}

Entity::~Entity() {}

void Entity::render(RenderContext& r, bool bind_shader) {
  if (m_material)
    m_material->bind(r, *this, bind_shader);
}

void Entity::update(float dt) {

}
