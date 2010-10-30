#include "settings.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "opengl.hpp"
#include "scene.hpp"
#include "entity.hpp"

#include <SDL.h>
#include <cstdlib>

#include <string>

struct TestEntity : public Entity {
  float m_time;
  TestEntity() : Entity(), m_time(0) {}

  void render(RenderContext& r, bool bind_shader = true) {
    glColor4f(1, 0.5, 0.7, 0.8);

    glBegin(GL_TRIANGLES);
    glVertex3f(-1, -1, -5 + cos(m_time));

    glVertex3f(1, -1, -5 + sin(m_time));
    glVertex3f(1, 1, -5);
    glEnd();
  }

  virtual void update(float dt) {
    m_time += dt;
  };
};

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

  bool fs = settings.get("full screen", false);

  int flags = fs ? SDL_FULLSCREEN : SDL_RESIZABLE;
  SDL_Surface* surface;
  SDL_Check(!(surface = SDL_SetVideoMode(settings.get("width", 800),
                                         settings.get("height", 600),
                                         0, SDL_OPENGL | flags)));

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    Log::error("glewItem failed: %s", glewGetErrorString(err));
    return 1;
  }
  Log::info("Using GLEW %s", glewGetString(GLEW_VERSION));

  if (fs)
    SDL_WM_GrabInput(SDL_GRAB_ON);
  SDL_ShowCursor(false);
  Scene * gameScene = new Scene();
  gameScene->root().reset(new TestEntity());
  Game game;
  game.setScene(gameScene);
  int ret = game.run();

  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(true);

  SDL_Quit();

  return ret;
}
