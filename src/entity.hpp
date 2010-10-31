#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "forward.hpp"
#include "zb_tr1.hpp"

#include <set>

class Renderable {
public:
  typedef std::set<RenderablePtr> Children;

  Renderable();
  virtual ~Renderable();

  virtual void render(RenderContext& r, bool bind_shader = true) = 0;
  virtual void update(float dt) = 0;
  Material* material() { return m_material; }


  Children & getChildren() { return m_children; }
  void addChild(RenderablePtr ch) { m_children.insert(ch); }
protected:
  Material* m_material;
private:
  Children m_children;
};

class Entity : public Renderable {
public:
  Entity();
  virtual ~Entity();

  void render(RenderContext& r, bool bind_shader = true);
  virtual void update(float dt);

protected:
  Model* m_model;
};

#endif // ENTITY_HPP
