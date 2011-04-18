#include "render_context.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "scene.hpp"
#include "material.hpp"
#include "utils.hpp"

#include <stack>
#include <string>

#include <LinearMath/btTransform.h>

RenderContext::RenderContext(Scene& scene) : m_scene(scene) {
}

void RenderContext::pushLights(Camera& camera) {
  glEnable(GL_LIGHTING);


  glEnable(GL_LIGHT0);
  GLfloat ambC[] = { 0.1, 0.0, 0.0, 1.0};
  GLfloat specC[] = { 1.0, 1.0, 1.0, 1.0};
  GLfloat diffC[] = { 0.0, 1.0, 0.0, 1.0};
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambC);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specC);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffC);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.000f);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.001f);

  btVector3 pos = camera.pos();
  pos.setW(1.0f);
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0f);
  btVector3 dir = camera.front().normalized();
  dir.setW(1.0f);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
}

void RenderContext::renderObjects(Camera& camera) {
  Objects& objs = objects(camera);
  btScalar transform[16];

  for (Objects::iterator oit = objs.begin(); oit != objs.end(); ++oit) {
    Set::iterator b = oit->second.begin(), e = oit->second.end();
    for (Set::iterator it = b; it != e; ++it) {
      RenderablePtr r = *it;
      const btTransform* m = r->worldTransform();
      if (m) {
        m->getOpenGLMatrix(transform);
        glPushMatrix();
        glMultMatrixf(transform);
      }
      r->render(*this, it == b);
      if (m) glPopMatrix();
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

void RenderContext::setBuffer(std::string name, int num) {
  m_buffers[name] = num;
}

void RenderContext::applyBuffers(unsigned int prog) {
  if (m_buffers.empty()) return;
  for (std::map<std::string, int>::iterator it = m_buffers.begin(); it != m_buffers.end(); ++it) {
    glRun(glBindFragDataLocation(prog, it->second, it->first.c_str()));
  }
  glLinkProgram(prog);
}
