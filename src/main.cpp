#include "settings.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "opengl.hpp"
#include "scene.hpp"
#include "entity.hpp"
#include "resource_manager.hpp"
#include "level.hpp"

#include <SDL.h>
#include <cstdlib>

#include <string>

struct TestEntity : public Entity {
  float m_time;
  TestEntity() : Entity(), m_time(0) {}

  void render(RenderContext& r, bool bind_shader = true) {
    /*
    glColor4f(0.5, 1, 0.3, 0.9);
    glBegin(GL_QUADS);
    glVertex3f(-50, -50, 2);
    glVertex3f(50, -50, -2);
    glVertex3f(50, 50, -2);
    glVertex3f(-50, 50, -2);
    glEnd();
    */


    glColor4f(1, 0.5, 0.7, 0.8);

    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f(-5, 15 + cos(m_time), -1);
    glVertex3f(1, 5 + sin(m_time), -1);
    glVertex3f(1, 5, 1);
    glVertex3f(15.5, 30, 0.5);
    glEnd();


  }

  virtual void update(float dt) {
    m_time += dt;
  };
};

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
  SDL_Check(!(surface = SDL_SetVideoMode(settings.get("width", 800),
                                         settings.get("height", 600),
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
  Scene * gameScene = new Scene();
  gameScene->root().reset(new Level());
  gameScene->root()->addChild(RenderablePtr(new TestEntity()));

  Game game;
  game.setSurface(surface);
  game.setScene(gameScene);
  int ret = game.run();

  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(true);

  SDL_Quit();

  return ret;
}
