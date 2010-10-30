#ifndef GAME_HPP
#define GAME_HPP

#include "forward.hpp"
#include "overlay.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "input_handler.hpp"

#include <list>

#include <SDL.h>

#define SDL_Check(r) do { Log::debug("Running: " #r); \
    if (r) sdl_error(#r); } while(0);

void sdl_error(const char* func);

/// Game engine main class
class Game {
public:
  enum GameState { MENU = 1 << 0,
                   GAME = 1 << 1 };

  Game();
  /// The main loop
  int run();

  static float gametime, dt;

  void setScene(Scene* scene) { m_scene = scene; }
  void setSurface(SDL_Surface* surface) { m_surface = surface; }

private:
  typedef std::list<Camera*> Cameras;

  Scene* m_scene;
  SDL_Surface* m_surface;
  Level* m_level;
  Overlay m_overlay;

  Player m_player;

  int m_game_state;
  bool m_running;

  Renderer m_renderer;
  Cameras m_cameras;

  InputHandler m_input;

  void handleEvents();
  void updatePhysics(float dt);
  void handleInput();

  void keyDown(SDLKey key);
  void keyUp(SDLKey key);
  void buttonDown(Uint8 btn, Uint16 x, Uint16 y);
  void buttonUp(Uint8 btn, Uint16 x, Uint16 y);
};

#endif // GAME_HPP
