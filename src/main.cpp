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

static void flatNormal(float normal[3], float v1[3], float v2[3], float v3[3])
{
  btVector3 v1_(v1[0], v1[1], v1[2]);
  btVector3 v2_(v2[0], v2[1], v2[2]);
  btVector3 v3_(v3[0], v3[1], v3[2]);
  btVector3 n = (v3_ - v1_).cross(v2_ - v1_).normalize();
  normal[0] = n.x(); normal[1] = n.y(); normal[2] = n.z();
}

struct TestEntity : public Entity {
  float m_time;
  TestEntity() : Entity(), m_time(0) {}

  void render(RenderContext& r, bool bind_shader = true) {
    glColor4f(1, 0.5, 0.7, 0.8);

    float vertices[][3] = {
      {-5, 15+5*cos(m_time), -1},
      {1, 5+4*sin(m_time), -1},
      {1, 5, 1},
      {15.5, 30, 0.5}
    };

    float normal[2][3];
    flatNormal(normal[0], vertices[0], vertices[1], vertices[2]);
    flatNormal(normal[1], vertices[3], vertices[2], vertices[1]);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i=0; i < 4; ++i) {
      glNormal3fv(normal[i/3]);
      glVertex3fv(vertices[i]);
    }
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


  Game game;
  game.setSurface(surface);

  Scene * gameScene = new Scene();
  gameScene->root() = game.level();
  gameScene->root()->addChild(RenderablePtr(new TestEntity()));

  game.setScene(gameScene);
  int ret = game.run();

  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(true);

  SDL_Quit();

  return ret;
}
