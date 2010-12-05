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
  // default implementation calls update on all children
  virtual void update(float dt);
  Material* material() { return m_material; }


  Children & getChildren() { return m_children; }
  void addChild(RenderablePtr ch) { m_children.insert(ch); }
  void setMaterial(Material * m) { m_material = m; }
protected:
  Material* m_material;
private:
  Children m_children;
};

class Entity : public Renderable {
public:
  Entity();
  virtual ~Entity();

  virtual void render(RenderContext& r, bool bind_shader = true);
  virtual void update(float dt);

  virtual bool load(const std::string& file);
  Model * getModel() { return m_model; }
protected:  
  Model* m_model;
};

class Enemy : public Entity {
public:
  Enemy();
  virtual ~Enemy();
  virtual void render(RenderContext &r, bool bind_shader = true);
  virtual void update(float dt);
};

#endif // ENTITY_HPP
