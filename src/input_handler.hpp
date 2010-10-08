#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP

#include "forward.hpp"

#include <set>
#include <map>
#include <string>

#include <SDL.h>

struct InputEvent {
public:
  /// Was the button pressed down?
  bool pressed;
  /// Was the button released since last time?
  bool released;
  /// is the button currently down
  bool down;

  InputEvent() : pressed(false), released(false), down(false) {}
  void clear() { pressed = released = false; }
};

class InputHandler {
public:
  InputHandler();
  ~InputHandler();

  /// Feed input
  static void keyDown(SDLKey key);
  static void keyUp(SDLKey key);
  static void mouseDown(Uint8 btn, Uint16 x, Uint16 y);
  static void mouseUp(Uint8 btn, Uint16 x, Uint16 y);

  static void clear();

  /// Output events for given action
  static InputEvent& event(const std::string& action);

  /// Key mappings
  static void setKey(SDLKey key, const std::string& action);
  /// Mouse button mappings
  static void setMouseBtn(Uint8 btn, const std::string& action);

private:
  typedef std::map<std::string, InputEvent> Events;
  typedef std::set<InputEvent*> Changed;

  typedef std::map<SDLKey, InputEvent*> BtnEvents;
  typedef std::map<SDLKey, std::string> BtnBindings;

  Events m_events;
  Changed m_changed;

  BtnEvents m_btn_events;
  BtnBindings m_btn_bindings;

  static InputHandler* s_instance;
};


#endif // INPUT_HANDLER_HPP
