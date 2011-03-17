#ifndef RENDER_CONTEXT_HPP
#define RENDER_CONTEXT_HPP

#include "forward.hpp"
#include "state.hpp"

#include <map>
#include <string>

class RenderContext : public State {
public:
  typedef std::set<RenderablePtr> Set;
  typedef std::map<GLProgram*, Set> Objects;

  RenderContext(Scene& scene);

  void pushLights(Camera& camera);
  void renderObjects(Camera& camera);
  void popLights();

  void setBuffer(std::string name, int num);

  Objects& objects(Camera& camera);

  void applyBuffers(unsigned int prog);
  void clearStuff() { m_buffers.clear(); }

protected:
  Scene& m_scene;
  typedef std::map<Camera*, Objects> Cache;
  Cache m_cache;
  std::map<std::string, int> m_buffers;
};


#endif // RENDER_CONTEXT_HPP
