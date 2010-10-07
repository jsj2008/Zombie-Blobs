#ifndef GAME_HPP
#define GAME_HPP

// Game engine main class
class Game {
public:
  Game();
  int run() { return 0; }
  /*mainloop {
    handleInput();

    if the current state includes normal game state {
      physicsUpdate(dt);
      for each entity, light, animator etc:
        .update(dt)
      scene.render();
      overlay.render();
    }

    if some kind of menu is visible {
      // GUI could be some kind of IMGUI-like, no update() or widgets etc
      menu.render();
    }

    swap();
  }
  enum GameState { MENU, GAME } state;
  Level* level;*/
};

#endif // GAME_HPP
