#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "forward.hpp"

/**
 * Renderer is given a list of renderable entities, and it groups them by
 * shader and material, and renders everything.
 */
class Renderer {
public:
  Renderer();

  void render(Scene* scene);
};

#endif // RENDERER_HPP
