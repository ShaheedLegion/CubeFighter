#ifndef _IMPL_H_
#define _IMPL_H_
#pragma once

#include "game_layer_stars.h"
#include "game_layer_twinkle.h"
#include "game_state_menu.h"
#include "game_state_start.h"
#include <map>

namespace impl {

// Game has all the stuff we need.
class Game {

  GameState currentState;
  std::map<GameState, GameScreen *> screens;

public:
  // Populate the map with all the correct game states.
  Game() : currentState(GAME_TITLE) {
    std::vector<GameLayer *> commonLayers;
    {
      // push the common layers into the vector here.
      commonLayers.push_back(new GameLayerStars());
      commonLayers.push_back(new GameLayerTwinkle());
    }
    {
      std::vector<GameState> startVec;
      startVec.push_back(GAME_MENU);
      screens[GAME_TITLE] =
          new GameScreenStart(GAME_TITLE, startVec, commonLayers);
    }

    {
      std::vector<GameState> menuVec;
      menuVec.push_back(GAME_TITLE);
      screens[GAME_MENU] = new GameScreenMenu(GAME_MENU, menuVec, commonLayers);
    }
  }

#if LEGACY
  // NB! Not called on the same thread as the render loop.
  void HandleKey(unsigned int key, bool pressed) {
    currentState = screens[currentState]->handleKeyInput(key, pressed);
  }

  // NB! Not called on the same thread as the render loop.
  void HandleMouse(int x, int y, bool pressed) {
    currentState = screens[currentState]->handleMouseInput(x, y, pressed);
  }
#endif // LEGACY

  // NB! Not called on the same thread as the input handlers.
  void Update(structures::InputState &state, unsigned int *buf, int w, int h) {
    currentState = screens[currentState]->handleInput(state);
    screens[currentState]->update(buf, w, h);
  }

  void GetOutput(std::vector<structures::textInfo> &text) {
    screens[currentState]->GetText(text);
  }
};

} // namespace impl

#endif _IMPL_H_