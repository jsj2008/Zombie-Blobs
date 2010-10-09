#include "renderer.hpp"
#include "fbo.hpp"
#include "texture.hpp"
#include "state.hpp"

RenderPass::RenderPass() {
}

int RenderPass::width() const {
  return m_width;
}

int RenderPass::height() const {
  return m_height;
}

void RenderPass::beginFBO() {
  if (!m_depth && !m_color)
    return;

  if (!m_fbo) {
    m_fbo.reset(new FrameBufferObject);
    if (m_depth) m_fbo->set(GL_DEPTH_ATTACHMENT, m_depth);
    if (m_color) m_fbo->set(GL_COLOR_ATTACHMENT0 + 0, m_color);
  }

  m_fbo->resize(width(), height());
  m_fbo->bind();
}

void RenderPass::endFBO() {
  if (m_fbo) m_fbo->unbind();
}


PostProc::PostProc() {
  m_viewport.setRect();
}

void PostProc::render(State& state) {
  beginFBO();

  m_shader.bind();

  /// @todo handle shader input here

  m_viewport.prepare(width(), height());

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);

  for (In::iterator it = m_in.begin(); it != m_in.end(); ++it) {
    TexturePtr tex = it->second;
    tex->bind(it->first);
    m_shader.setUniform(tex->name(), it->first);
  }
  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0.0f, 0.0f);

  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(width(), 0.0f);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(width(), height());

  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0.0f, height());

  glEnd();

  for (In::iterator it = m_in.begin(); it != m_in.end(); ++it)
    it->second->unbind();

  m_shader.unbind();

  endFBO();
}

SceneRenderer::SceneRenderer() : m_clear(0) {}

void SceneRenderer::render(State& state) {
  beginFBO();

  m_viewport.prepare(width(), height());

  if (m_clear) glClear(m_clear);

  state.enable(GL_DEPTH_TEST);
  state.enable(GL_CULL_FACE);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glShadeModel(GL_SMOOTH);

  /// @todo activate all lights

  /// @todo collect all objects and render them

  /// @todo deactivate the lights

  endFBO();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Renderer::Renderer() : m_width(0), m_height(0) {
}

void Renderer::render(Scene* scene) {

}
