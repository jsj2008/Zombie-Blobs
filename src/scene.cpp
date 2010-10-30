#include "scene.hpp"
#include "entity.hpp"

Scene::Scene() : m_grid(100.0f, 100.0f, 64, 64) {
}

void Scene::update(float dt) {
  if (!m_root)
    return;

  m_root->update(dt);
}
