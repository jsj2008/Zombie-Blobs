#include "game.hpp"
#include "settings.hpp"
#include "scene.hpp"
#include "input_handler.hpp"
#include "level.hpp"

#include <cmath>

float Game::gametime = 0.0f;
float Game::dt = 0.0f;
Game* Game::s_instance = 0;

void sdl_error(const char* func) {
 Log::error("SDL error: %s: %s", func, SDL_GetError());
 exit(EXIT_FAILURE);
}

Game::Game() : m_scene(0), m_surface(0), m_level(new Level()),
    m_player(new Player()), m_game_state(GAME), m_running(true) {
  assert(!s_instance);
  s_instance = this;
  m_cameras.push_back(m_player);

  m_renderer.setupPasses();
  SDL_Surface * surf = SDL_GetVideoSurface();
  if (surf)
    m_renderer.resize(surf->w, surf->h);

  InputHandler::setKey(SDLK_ESCAPE, "quit");
}

int Game::run() {
  Uint32 start_ticks = SDL_GetTicks();
  Uint32 previous_ticks = start_ticks;

  while (m_running) {
    Uint32 ticks = SDL_GetTicks();
    Uint32 delta_ticks = ticks - previous_ticks;

    gametime = (ticks - start_ticks) / 1000.0f;
    dt = delta_ticks / 1000.0f;

    handleEvents();
    handleInput();

    if ((m_game_state & GAME) && m_scene) {
      updatePhysics(dt);
      m_scene->update(dt);
      m_renderer.render(*m_scene);
      m_overlay.render(m_game_state, dt);
    }

    if (m_game_state & MENU) {


      // render menu
    }
    SDL_GL_SwapBuffers();

    previous_ticks += delta_ticks;

    // Limit FPS
    Uint32 delay = 1000u / Settings::get("fps limit", 60u);
    delta_ticks = SDL_GetTicks() - ticks;
    if (delta_ticks < delay) {
      /// @todo try to run physics here (or in a separate thread)
      SDL_Delay(delay - delta_ticks);
    }
  }

  return 0;
}

void Game::handleEvents() {
  float max_rel = Settings::get("max cursor move", 70.0f);
  float mouse_sensitivity = Settings::get("mouse sensitivity", 0.3f);
  float mouse_acceleration = Settings::get("mouse acceleration", 1.1f);

  InputHandler::clear();

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type)	{
      case SDL_QUIT:
        m_running = false;
        break;

      case SDL_KEYDOWN:
        InputHandler::keyDown(event.key.keysym.sym);
        break;

      case SDL_KEYUP:
        InputHandler::keyUp(event.key.keysym.sym);
        break;

      case SDL_MOUSEMOTION: {
        float xrel = event.motion.xrel;
        float yrel = event.motion.yrel;

        // Sometimes (like when setting mouse grabbing on/off and initializing
        // a new window) SDL creates unwanted mousemotion events that aren't real
        // but only a consequence of a a forced cursor reposition to center to
        // the screen or something similar.
        //
        // So do not allow too big events.
        // Too bad if you have a slow computer.
        if (xrel < -max_rel || xrel > max_rel || yrel < -max_rel || yrel > max_rel)
          break;

        float tmp = std::pow(std::abs(xrel)*mouse_sensitivity, mouse_acceleration);
        xrel = xrel < 0.0f ? -tmp : tmp;
        tmp = std::pow(std::abs(yrel)*mouse_sensitivity, mouse_acceleration);
        yrel = yrel < 0.0f ? -tmp : tmp;

        m_player->rotate(xrel, yrel);
        break;
      }

      case SDL_MOUSEBUTTONDOWN:
        InputHandler::mouseDown(event.button.button, event.button.x, event.button.y);
        break;

      case SDL_MOUSEBUTTONUP:
        InputHandler::mouseUp(event.button.button, event.button.x, event.button.y);
        break;

      case SDL_VIDEORESIZE:
        SDL_Check(!(m_surface = SDL_SetVideoMode(event.resize.w, event.resize.h,
                                                 0, m_surface->flags)));
        m_renderer.resize(event.resize.w, event.resize.h);
        break;
    }
  }
}

void Game::handleInput() {
  static InputEvent& quit = InputHandler::event("quit");
  if(quit.pressed)
    m_running = false;
}

void Game::updatePhysics(float dt) {

}

