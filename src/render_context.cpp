#include "render_context.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "scene.hpp"
#include "material.hpp"

#include <stack>

RenderContext::RenderContext(Scene& scene) : m_scene(scene) {
}

void RenderContext::pushLights(Camera& camera) {
  glEnable(GL_LIGHTING);
  GLfloat lightC[] = { 0.2, 0.4, 0.6, 1.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightC);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightC);

  glEnable(GL_LIGHT0);
  GLfloat ambC[] = { 0.0, 0.0, 1.0, 1.0};
  GLfloat specC[] = { 1.0, 0.0, 0.0, 1.0};
  GLfloat diffC[] = { 0.0, 1.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambC);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specC);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffC);
  glLightfv(GL_LIGHT0, GL_POSITION, camera.pos().m_floats);
//  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 35.0f);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, camera.front().normalized().m_floats);
  glMateriali(GL_FRONT, GL_SHININESS, 128);
}

void RenderContext::renderObjects(Camera& camera) {
  Objects& objs = objects(camera);

  for (Objects::iterator oit = objs.begin(); oit != objs.end(); ++oit) {
    Set::iterator b = oit->second.begin(), e = oit->second.end();
    for (Set::iterator it = b; it != e; ++it) {
      RenderablePtr r = *it;
      r->render(*this, it == b);
    }
  }
}

void RenderContext::popLights() {
	glDisable(GL_LIGHTING);
}

// FIXME: use grid
RenderContext::Objects& RenderContext::objects(Camera& camera) {

  Cache::iterator it = m_cache.find(&camera);
  if (it != m_cache.end())
    return it->second;

  Objects& objs = m_cache[&camera];

#if 0
  m_scene.grid().objects(objs, camera);
#else // just collect all renderables
  std::stack<RenderablePtr> s;
  s.push(m_scene.root());
  while (!s.empty()) {
    RenderablePtr r = s.top();
    s.pop();
    if (!r)
      continue;
    objs[r->material() ? r->material()->shader() : 0].insert(r);
    Renderable::Children ch = r->getChildren();

    for (Renderable::Children::iterator it = ch.begin();
      it != ch.end(); ++it) {
      s.push(*it);
    }
  }
#endif
  return objs;
}
