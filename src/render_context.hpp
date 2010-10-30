#ifndef RENDER_CONTEXT_HPP
#define RENDER_CONTEXT_HPP

#include "forward.hpp"
#include "state.hpp"

#include <map>

class RenderContext : public State {
public:
  typedef std::set<RenderablePtr> Set;
  typedef std::map<GLProgram*, Set> Objects;

  RenderContext(Scene& scene);

  void pushLights(Camera& camera);
  void renderObjects(Camera& camera);
  void popLights();

  Objects& objects(Camera& camera);
protected:
  Scene& m_scene;
  typedef std::map<Camera*, Objects> Cache;
  Cache m_cache;
};


#endif // RENDER_CONTEXT_HPP
