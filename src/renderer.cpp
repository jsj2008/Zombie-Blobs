#include "renderer.hpp"
#include "fbo.hpp"
#include "texture.hpp"
#include "state.hpp"
#include "render_context.hpp"
#include "game.hpp"

RenderPass::RenderPass() :
    m_width(0), m_height(0), m_viewport(new Camera()) {
}
RenderPass::~RenderPass() {}

int RenderPass::width() const {
  return m_width;
}

int RenderPass::height() const {
  return m_height;
}

void RenderPass::beginFBO() {
  if (!m_depth && m_colors.empty())
    return;

  if (!m_fbo) {
    m_fbo.reset(new FrameBufferObject);
    if (m_depth) m_fbo->set(GL_DEPTH_ATTACHMENT, m_depth);
    FBOImageList::iterator it = m_colors.begin();
    int i=0;
    while (it != m_colors.end()) {
      m_fbo->set(GL_COLOR_ATTACHMENT0 + i++, *it);
      ++it;
    }
    //if (m_color) m_fbo->set(GL_COLOR_ATTACHMENT0 + 0, m_color);
  }

  m_fbo->resize(width(), height());
  m_fbo->bind();
}

void RenderPass::endFBO() {
  if (m_fbo) m_fbo->unbind();
}


PostProc::PostProc() {
  m_viewport->setRect();
}

PostProc::~PostProc() {}

void PostProc::render(RenderContext& r) {
  r.push();
  beginFBO();

  if (!m_shader.shaders().empty())
    m_shader.bind();

  /// @todo handle shader input here

  m_viewport->prepare(width(), height());

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);

  for (In::iterator it = m_in.begin(); it != m_in.end(); ++it) {
    TexturePtr tex = it->second;

    int unit = r.reserveTexUnit();
    tex->bind(unit);

    if (!m_shader.isLinked())
      continue;

    m_shader.setUniform(it->first, unit);
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

  if (!m_shader.shaders().empty())
    m_shader.unbind();

  endFBO();
  r.pop();
}

SceneRenderPass::SceneRenderPass() : m_clear(0) {}

SceneRenderPass::~SceneRenderPass() {}

void SceneRenderPass::render(RenderContext& r) {
  beginFBO();  

  m_viewport->prepare(width(), height());
  glClearColor(0.1, 0.4, 0.7, 0.8);
  if (m_clear) glClear(m_clear);
  glViewport(0, 0, width(), height());
  r.enable(GL_DEPTH_TEST);
  r.enable(GL_CULL_FACE);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glShadeModel(GL_SMOOTH);



  r.pushLights(*m_viewport);
  r.renderObjects(*m_viewport);
  r.popLights();

  endFBO();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Renderer::Renderer() : m_width(0), m_height(0) {
}

/// FIXME: this is wrong
void Renderer::resize(int w, int h) {

  m_width = w;
  m_height = h;
  for (std::list<RenderPassPtr>::iterator it = m_render_passes.begin();
       it != m_render_passes.end(); ++it) {
    (*it)->resize(w, h);
  }
}

void Renderer::render(Scene& scene) {
  RenderContext r(scene);
  for (std::list<RenderPassPtr>::iterator it = m_render_passes.begin();
       it != m_render_passes.end(); ++it) {
    (*it)->render(r);
  }
}

void Renderer::setupPasses() {
  SceneRenderPass* scene = new SceneRenderPass;
  scene->setClearBits(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  TexturePtr tex(new Texture());
  TexturePtr normals(new Texture());
  scene->m_colors.push_back(tex);
  scene->m_colors.push_back(normals);
  TexturePtr depth(new Texture());

  scene->m_depth = depth;

  PostProc* post = new PostProc();
  post->m_in["scene"] = tex;
  post->m_in["sceneDepth"] = depth;
  post->m_in["normals"] = normals;
  if (!post->m_shader.addShader("postproc.fs", Shader::Fragment))
    Log::error("could not load postproc shader postproc.fs");

  PlayerPtr player = Game::instance()->player();
  scene->m_viewport = player;

  m_render_passes.push_back(RenderPassPtr(scene));
  m_render_passes.push_back(RenderPassPtr(post));
}
