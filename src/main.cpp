#include "settings.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "opengl.hpp"

#include <SDL/SDL.h>
#include <cstdlib>

#include <string>

#define SDL_Check(r) do { Log::debug("Running: " #r); \
    if (r) sdl_error(#r); } while(0);

void sdl_error(const char* func) {
  Log::error("SDL error: %s: %s", func, SDL_GetError());
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  Log log;
  Settings settings(argc, argv);

  Log::info("Starting game");

  SDL_Check(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER));
  SDL_Check(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
  SDL_Check(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16));

  SDL_WM_SetCaption("Zombie-Blobs", "Zombie-Blobs");

  SDL_Surface* surface;
  SDL_Check(!(surface = SDL_SetVideoMode(settings.get("width", 800),
                                         settings.get("height", 600),
                                         0, SDL_OPENGL | SDL_RESIZABLE)));

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    Log::error("glewItem failed: %s", glewGetErrorString(err));
    return 1;
  }

  Log::info("Using GLEW %s", glewGetString(GLEW_VERSION));

  Game game;
  int ret = game.run();

  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(true);

  SDL_Quit();

  return ret;
}
