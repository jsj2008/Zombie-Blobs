#include "renderer.hpp"
#include "fbo.hpp"
#include "texture.hpp"
#include "state.hpp"
#include "render_context.hpp"
#include "game.hpp"
#include "level.hpp"
#include "opengl.hpp"
#include "math.hpp"
#include "model.hpp"
#include "player.hpp"

class Random : public Texture
{
protected:
  void texImage(int width, int height) {
    srand(time(0));
    float f[64*64*4];
    float numDirs = 12.0f;
    for(int i=0; i<64*64*4; i+=4) {
      float angle = 2.0f * M_PI * (rand() / float(RAND_MAX)) / numDirs;
      f[i  ] = cosf(angle);
      f[i+1] = sinf(angle);
      f[i+2] = rand() / float(RAND_MAX);
      f[i+3] = 0;
    }

    glRun(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 64, 64,
                       0, GL_RGBA, GL_FLOAT, f));
  }
};

RenderPass::RenderPass() :
    m_width(0), m_height(0), m_viewport(new Camera()) {
  m_scale[0] = m_scale[1] = 1.0f;
}
RenderPass::~RenderPass() {}

int RenderPass::width() const {
  return m_width * m_scale[0];
}

int RenderPass::height() const {
  return m_height * m_scale[1];
}

void RenderPass::beginFBO(RenderContext& r) {
  if (!m_depth && m_out.empty())
    return;

  if (!m_fbo) {
    m_fbo.reset(new FrameBufferObject);
    if (m_depth) m_fbo->set(GL_DEPTH_ATTACHMENT, -1, m_depth);
    FBOImageList::iterator it = m_out.begin();
    int i=0;
    while (it != m_out.end()) {
      r.setBuffer(it->first, i);
      m_fbo->set(GL_COLOR_ATTACHMENT0 + i, i, it->second);
      ++it, ++i;
    }
  }

  m_fbo->resize(width(), height());
  m_fbo->bind(r);
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
  beginFBO(r);

  if (!m_shader.shaders().empty())
    m_shader.bind(r);

  /// @todo handle shader input here

  m_viewport->prepare(width(), height());

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);

  for (In::iterator it = m_in.begin(); it != m_in.end(); ++it) {
    TexturePtr tex = it->second;

    int unit = r.reserveTexUnit();
    glCheck("bind");
    tex->bind(unit, width(), height());
    glCheck("aftbind");


    if (!m_shader.isLinked())
      continue;

    m_shader.setUniform(r, it->first, unit);

  }

  glBegin(GL_QUADS);

  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(0.0f, 0.0f);

  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(float(width()), 0.0f);

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(float(width()), float(height()));

  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(0.0f, float(height()));

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
  r.enable(GL_BLEND);
  beginFBO(r);

  m_viewport->prepare(width(), height());
  glClearColor(0, 0, 0, 1);
  if (m_clear) glClear(m_clear);
  glViewport(0, 0, width(), height());
  r.enable(GL_DEPTH_TEST);
  r.enable(GL_CULL_FACE);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glShadeModel(GL_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  r.pushLights(*m_viewport);
  r.renderObjects(*m_viewport);
  r.popLights();


  endFBO();
  r.disable(GL_BLEND);
}

HudRenderPass::HudRenderPass() : RenderPass() {
  m_viewport->setRect();
}
HudRenderPass::~HudRenderPass() {}

void HudRenderPass::render(RenderContext &r)
{
  btVector3 vel = Game::instance()->player()->getVelocity();

  r.push();
  beginFBO(r);

  r.disable(GL_DEPTH_TEST);
  r.disable(GL_LIGHTING);

  r.enable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  m_viewport->prepare(width(), height()); // normalized coords

  // 60 units is one round
  float speed = (vel.length() / 60) * (2*3.145926f) ;

  btVector3 needleBase(0.2f, 0.2f, 0);
  float radius = 0.1f;

  btVector3 needle[3];
  float theta = -(3/4.0f)*M_PI - speed;
  needle[0] = radius * btVector3(cos(theta), sin(theta), 0) + needleBase;

  btVector3 diff = needle[0] - needleBase;
  diff.normalize();
  diff = diff.rotate(btVector3(0, 0, 1), M_PI_2);
  diff *= 0.01;

  needle[1] = needleBase + diff;
  needle[2] = needleBase - diff;
  for (int i=0; i < 3; ++i) {
    needle[i].setX(needle[i].x() * width());
    needle[i].setY(needle[i].y() * height());
  }

  glColor4f(speed/(2*M_PI), 0, 0, 0.5);

  glBegin(GL_TRIANGLES);
  for (int i=0; i<3; ++i)
    glVertex2fv(needle[i].m_floats);
  glEnd();

  /* Draw a map */
  btVector3 * bb = Game::instance()->level()->aabb();
  btVector3 size = bb[1] - bb[0];
  btAlignedObjectArray<Enemy*> enemies = Game::instance()->level()->getEnemies();

  // :(
  size.setX(1000);
  size.setY(1000);
  size.setZ(100);

  float aspect = size.x() / size.y();

  const float w = 0.2f;
  const float h = w*aspect;
  float padX = 0.02f;
  float padY = padX*aspect;


  glPushMatrix();
  // scale and translate so that map bounding box is contained in
  // [1-w-pad, 1-pad] x [pad, pad+h]

  glScalef(width(), width(), 1.0f);
  glTranslatef( 1 - w - padX, padY, 0);
  //glScalef(w, h, 1);
  glScalef(w/size.x(), h/size.y(), 1.0f);

  /*
  glColor4f(1,1,1,0.1);
  glBegin(GL_QUADS);
  for (int i=0; i < 4; ++i) {
    glVertex2f(0, 0);
    glVertex2f(size.x(), 0);
    glVertex2f(size.x(), size.y());
    glVertex2f(0, size.y());
  }
  glEnd();
  */

  glColor4f(1,1,1,0.2);
  TexturePtr tex = Game::instance()->level()->mapTexture();  
  tex->bind();

  glBlendFuncSeparate(
      GL_ONE_MINUS_SRC_COLOR,
      GL_SRC_COLOR,
      GL_ONE,
      GL_ONE
      );

  glBegin(GL_QUADS);
  for (int i=0; i < 4; ++i) {
    glTexCoord2f(0, 0);
    glVertex2f(0, 0);

    glTexCoord2f(1, 0);
    glVertex2f(size.x(), 0);

    glTexCoord2f(1, 1);
    glVertex2f(size.x(), size.y());

    glTexCoord2f(0, 1);
    glVertex2f(0, size.y());
  }
  glEnd();
  tex->unbind();

  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  PlayerPtr player = Game::instance()->player();
  btVector3 ppos = 2*player->getPosition();
  btVector3 pdir = player->front();

  btVector3 pdirXY(pdir.x(), pdir.y(), 0);
  pdirXY.normalize();

  // fov in radians
  float fov = player->fov() * (M_PI / 180.f);
  btVector3 side1 = pdirXY.rotate(btVector3(0, 0, 1), fov);
  btVector3 side2 = pdirXY.rotate(btVector3(0, 0, 1), -fov);




  glBegin(GL_TRIANGLES);
  glColor4f(1, 1, 0, 0.7);
  glVertex2fv( ppos.m_floats );
  glColor4f(1, 1, 0, 0);
  glVertex2fv( (ppos+side2*2*player->fardist()).m_floats);
  glVertex2fv( (ppos+side1*2*player->fardist()).m_floats);
  glEnd();



  glColor4f(0, 1, 0, 1);
  glPointSize(2);
  glBegin(GL_POINTS);
  for (int i=0; i < enemies.size(); ++i) {
    Enemy& enemy = *enemies[i];
    btVector3 orig = 2*enemy.getModel()->getCollisionObject()->getWorldTransform().getOrigin();
    glVertex2fv(orig.m_floats);
  }
  glEnd();

  /*
  btAlignedObjectArray<btVector3> corners;
  player->frustumCorners(corners);

  for (int k=0; k < 2; ++k) {
    glBegin(GL_LINE_LOOP);
    for (int i=0; i < 4; ++i) {
      glVertex2fv( (2*corners[i+k*4]).m_floats);
    }
    glEnd();
  }
  */

  glColor4f(1, 0, 0, 1);
  glPointSize(5);
  glBegin(GL_POINTS); 
  glVertex2fv(ppos.m_floats);
  glEnd();

  glPopMatrix();

  r.disable(GL_BLEND);

  endFBO();
  r.pop();
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
  // 1st pass, render scene to textures:
  // color, face normals and linearized depth buffer
  SceneRenderPass* scene = new SceneRenderPass;
  scene->setClearBits(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

  TexturePtr diffuse(new Texture);
  // raw face normals, not vertex/interpolated normals
  TexturePtr normals(new Texture);
  normals->setInternalFormat(GL_RGB32F);
  normals->setTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  normals->setTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // linearized depth buffer
  TexturePtr lindepth(new Texture);
  lindepth->setInternalFormat(GL_R32F);
  scene->m_out["diffuse"] = diffuse;
  scene->m_out["normals"] = normals;
  scene->m_out["lindepth"] = lindepth;
  diffuse->setBufferState(GL_BLEND, false);
  normals->setBufferState(GL_BLEND, false);
  lindepth->setBufferState(GL_BLEND, false);
  scene->m_depth.reset(new RenderBuffer);


  // 2nd pass, render ssao to texture, with half the size of scene pass
  PostProc* ssao = new PostProc;
  ssao->setScale(0.5f, 0.5f);

  TexturePtr ao(new Texture);
  ao->setInternalFormat(GL_R32F);
  ssao->m_out["value"] = ao;
  TexturePtr random(new Random);
  random->setTextureParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
  random->setTextureParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
  random->setTextureParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  random->setTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  ssao->m_in["lindepth"] = lindepth;
  ssao->m_in["normals"] = normals;
  ssao->m_in["random"] = random;
  if (!ssao->m_shader.addShader("ao.vs", Shader::Vertex))
    Log::error("could not load postproc shader ao.vs");
  if (!ssao->m_shader.addShader("ao.fs", Shader::Fragment))
    Log::error("could not load postproc shader ao.fs");


  // 3rd pass, depth-dependent gaussian blur to ao, full resolution (X-direction)
  PostProc* blurx = new PostProc;

  TexturePtr tmp(new Texture);
  tmp->setInternalFormat(GL_R32F);
  blurx->m_out["blurred"] = tmp;

  blurx->m_in["lindepth"] = lindepth;
  blurx->m_in["image"] = ao;
  if (!blurx->m_shader.addShader("blurx.fs", Shader::Fragment))
    Log::error("could not load postproc shader blurx.fs");


  // 4th pass, depth-dependent gaussian blur (Y-direction),
  // also combine the scene color buffer with the blurred ao
  PostProc* blury = new PostProc;

  blury->m_in["lindepth"] = lindepth;
  blury->m_in["diffuse"] = diffuse;
  blury->m_in["blurred"] = tmp;
  if (!blury->m_shader.addShader("blury.fs", Shader::Fragment))
    Log::error("could not load postproc shader blury.fs");


  PlayerPtr player = Game::instance()->player();
  scene->m_viewport = player;

  HudRenderPass * hud = new HudRenderPass();


  PostProc* debug = new PostProc;

  debug->m_in["diffuse"] = diffuse;
  debug->m_in["normals"] = normals;
  debug->m_in["lindepth"] = lindepth;
  debug->m_in["ao"] = ao;
  if (!debug->m_shader.addShader("debug.fs", Shader::Fragment))
    Log::error("could not load postproc shader debug.fs");

  m_render_passes.push_back(RenderPassPtr(scene));
  m_render_passes.push_back(RenderPassPtr(ssao));
  m_render_passes.push_back(RenderPassPtr(blurx));
  m_render_passes.push_back(RenderPassPtr(blury));
  m_render_passes.push_back(RenderPassPtr(hud));
  m_render_passes.push_back(RenderPassPtr(debug));
}
