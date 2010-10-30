#ifndef SCENE_HPP
#define SCENE_HPP

#include "forward.hpp"
#include "grid.hpp"
#include "zb_tr1.hpp"

/***
  * Scene includes all renderable 3D-objects, lights, the generated level etc.
  *
  * It's not a SceneGraph, but entities have a parent/child relationship and
  * if needed, we can have many render layers.
  *
  * Also includes triggers / animators?
  */
class Scene {
public:

  Scene();

  void update(float dt);

  Grid& grid() { return m_grid; }
  RenderablePtr & root() { return m_root; }
protected:
  RenderablePtr m_root;
  Grid m_grid;
};

#endif // SCENE_HPP
