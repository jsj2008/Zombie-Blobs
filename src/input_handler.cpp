#include "input_handler.hpp"

#include <cassert>

InputHandler* InputHandler::s_instance = 0;

InputHandler::InputHandler() {
  if (!s_instance)
    s_instance = this;
}

InputHandler::~InputHandler() {
  if (s_instance == this)
    s_instance = 0;
}

void InputHandler::keyDown(SDLKey key) {
  assert(s_instance);

  BtnEvents::iterator it = s_instance->m_btn_events.find(key);
  if (it != s_instance->m_btn_events.end()) {
    InputEvent* e = it->second;
    e->down = true;
    e->pressed = true;
    s_instance->m_changed.insert(e);
  }
}

void InputHandler::keyUp(SDLKey key) {
  assert(s_instance);

  BtnEvents::iterator it = s_instance->m_btn_events.find(key);
  if (it != s_instance->m_btn_events.end()) {
    InputEvent* e = it->second;
    e->down = false;
    e->released = true;
    s_instance->m_changed.insert(e);
  }
}

void InputHandler::mouseDown(Uint8 btn, Uint16 /*x*/, Uint16 /*y*/) {
  keyDown(SDLKey(SDLK_LAST + btn));
}

void InputHandler::mouseUp(Uint8 btn, Uint16 /*x*/, Uint16 /*y*/) {
  keyUp(SDLKey(SDLK_LAST + btn));
}

void InputHandler::clear() {
  assert(s_instance);

  for (Changed::const_iterator it = s_instance->m_changed.begin();
       it != s_instance->m_changed.end(); ++it) {
    (*it)->clear();
  }
  s_instance->m_changed.clear();
}

InputEvent& InputHandler::event(const std::string& action) {
  assert(s_instance);

  InputEvent& e = s_instance->m_events[action];

  for (BtnBindings::const_iterator it = s_instance->m_btn_bindings.begin();
       it != s_instance->m_btn_bindings.end(); ++it) {
    if (it->second == action)
      s_instance->m_btn_events[it->first] = &e;
  }

  return e;
}

void InputHandler::setKey(SDLKey key, const std::string& action) {
  assert(s_instance);

  s_instance->m_btn_bindings[key] = action;

  Events::iterator it = s_instance->m_events.find(action);
  if (it != s_instance->m_events.end()) {
    s_instance->m_btn_events[key] = &it->second;
  } else {
    s_instance->m_btn_events.erase(key);
  }
}

void InputHandler::setMouseBtn(Uint8 btn, const std::string& action) {
  setKey(SDLKey(SDLK_LAST + btn), action);
}
