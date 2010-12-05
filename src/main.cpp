#include "settings.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "opengl.hpp"
#include "scene.hpp"
#include "entity.hpp"
#include "resource_manager.hpp"
#include "level.hpp"
#include "model.hpp"

#include <SDL.h>
#include <cstdlib>

#include <string>

int main(int argc, char* argv[]) {
  Log log;
  Settings settings(argc, argv);
  ResourceManager resources;
  resources.addPath(".");

  Log::info("Starting game");

  SDL_Check(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER));
  SDL_Check(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
  SDL_Check(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16));

  SDL_WM_SetCaption("Zombie-Blobs", "Zombie-Blobs");

  bool fs = settings.get("full screen", false);

  int flags = fs ? SDL_FULLSCREEN : SDL_RESIZABLE;
  SDL_Surface* surface;
  SDL_Check(!(surface = SDL_SetVideoMode(settings.get("width", 1280),
                                         settings.get("height", 800),
                                         0, SDL_OPENGL | flags)));

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    Log::error("glewItem failed: %s", glewGetErrorString(err));
    return 1;
  }
  Log::info("Using GLEW %s", glewGetString(GLEW_VERSION));
  if (true || fs)
    SDL_WM_GrabInput(SDL_GRAB_ON);

  SDL_ShowCursor(false);


  Game game;
  game.setSurface(surface);
  game.init();

  Scene * gameScene = new Scene();
  gameScene->root() = game.level();

  for (int i=0; i < 100 ; ++i) {
    game.level()->spawnNewEnemy();
  }

  game.setScene(gameScene);
  int ret = game.run();

  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(true);

  SDL_Quit();

  return ret;
}
