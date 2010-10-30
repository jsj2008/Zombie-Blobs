#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "forward.hpp"
#include "shader.hpp"
#include "camera.hpp"

#include <set>
#include <map>

/**
 * Render pass represents one renderable image that might only be a small part
 * of the whole composition. Different kinds of render passes could be for
 * example shadow map rendering, gaussian blur post-processing and normal phong
 * shading.
 *
 * Render pass might use outputs (textures) of other render passes as inputs
 * to shaders, and the output generated from this render pass might be either
 * the final output to screen or an intermediate result, rendered to texture
 * with Frame Buffer Object.
 *
 * Currently one render pass can have exactly one shader program and one Camera
 * (or more generally "viewport"). It also has list of objects, lights and
 * in/out textures.
 */
class RenderPass {
public:
  RenderPass();
  virtual ~RenderPass();

  int height() const;
  int width() const;
  void resize(int w, int h) { m_width = w; m_height = h; }

  /// Render the pass
  virtual void render(RenderContext& r) = 0;

protected:
  void beginFBO();
  void endFBO();

  int m_width, m_height; /// Output size

  FBOPtr m_fbo;
  FBOImagePtr m_depth, m_color;

  CameraPtr m_viewport;
  friend class Renderer;
};

class PostProc : public RenderPass {
public:
  PostProc();
  virtual ~PostProc();

  void render(RenderContext& r);

protected:
  typedef std::map<int, TexturePtr> In;
  In m_in;

  GLProgram m_shader;
  friend class Renderer;
};

class SceneRenderPass : public RenderPass {
public:
  SceneRenderPass();
  virtual ~SceneRenderPass();

  GLbitfield clearBits() const { return m_clear; }
  void setClearBits(GLbitfield bits) { m_clear = bits; }

  void render(RenderContext& r);

protected:
  /// Bitwise OR of GL_{COLOR,DEPTH,STENCIL}_BUFFER_BIT, or zero if we don't
  /// want to clear the buffer before rendering.
  GLbitfield m_clear;
};


/**
 * Renderer is given a list of renderable entities, and it groups them by
 * shader and material, and renders everything.
 */

class Renderer {
public:
  Renderer();

  void resize(int w, int h);
  void render(Scene& scene);

  void setupPasses();

private:
  int m_width, m_height;
  std::list<RenderPassPtr> m_render_passes;
};

#endif // RENDERER_HPP
