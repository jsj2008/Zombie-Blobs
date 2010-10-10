#include "render_context.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "scene.hpp"

RenderContext::RenderContext(Scene& scene) : m_scene(scene) {
}

void RenderContext::pushLights(Camera& camera) {

}

void RenderContext::renderObjects(Camera& camera) {
  Objects& objs = objects(camera);

  for (Objects::iterator oit = objs.begin(); oit != objs.end(); ++oit) {
    Set::iterator b = oit->second.begin(), e = oit->second.end();
    for (Set::iterator it = b; it != e; ++it) {
      Renderable* r = *it;
      r->render(*this, it == b);
    }
  }
}

void RenderContext::popLights() {

}

RenderContext::Objects& RenderContext::objects(Camera& camera) {
  Cache::iterator it = m_cache.find(&camera);
  if (it != m_cache.end())
    return it->second;

  Objects& objs = m_cache[&camera];
  m_scene.grid().objects(objs, camera);

  return objs;
}
