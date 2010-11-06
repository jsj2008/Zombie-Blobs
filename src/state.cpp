#include "state.hpp"
#include "utils.hpp"

State::State() {
  m_data.push(Data());
}

int State::nextFreeLight() const {
  return nextFree(m_data.top().m_lights);
}

void State::setLight(int light_id, bool in_use) {
  if (in_use) {
    m_data.top().m_lights.insert(light_id);
    enable(GL_LIGHTING);
    enable(GL_LIGHT0 + light_id);
  } else {
    std::set<int> foo;
    m_data.top().m_lights.erase(light_id);
    disable(GL_LIGHT0 + light_id);
  }
}

void State::enable(GLenum cap) {
  glEnable(cap);
}

void State::disable(GLenum cap) {
  glDisable(cap);
}

int State::reserveTexUnit() {
  int unit = nextFree(m_data.top().m_texunits);
  m_data.top().m_texunits.insert(unit);
  return unit;
}

void State::push() {
  m_data.push(m_data.top());
}

void State::pop() {
  if (m_data.size() <= 1) {
    Log::error("State push/pop mismatch");
  }
}

int State::nextFree(const std::set<int>& set, int id) const {
  while (set.count(id)) ++id;
  return id;
}
