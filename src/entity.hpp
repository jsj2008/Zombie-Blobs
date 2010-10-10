#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "forward.hpp"

class Renderable {
public:
  Renderable();
  virtual ~Renderable();

  virtual void render(RenderContext& r, bool bind_shader = true) = 0;
  Material* material() { return m_material; }

protected:
  Material* m_material;
};

class Entity : public Renderable {
public:
  Entity();
  virtual ~Entity();

  void render(RenderContext& r, bool bind_shader = true);

protected:
  Model* m_model;
};

#endif // ENTITY_HPP
