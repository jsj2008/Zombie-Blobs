#ifndef SCENE_HPP
#define SCENE_HPP

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
};

#endif // SCENE_HPP
