#ifndef _GAME_SCREEN_H_
#define _GAME_SCREEN_H_
#pragma once

#include "layers.h"
#include "sin_counter.h"
#include "structures.h"
#include "texture_loader.h"

namespace impl {

enum GameState {
  GAME_TITLE = 0,
  GAME_MENU,
  GAME_PLAY,
  GAME_OPTIONS,
  GAME_HIGHSCORES,
  GAME_CREDITS,
  GAME_QUIT,
};

class GameScreen {
protected:
  GameState myState;
  std::vector<GameState> transitions;
  std::vector<GameLayer *> layers;
  std::vector<structures::textInfo> screenText;

public:
  GameScreen(GameState state, const std::vector<GameState> &otherStates,
             const std::vector<GameLayer *> &layers)
      : myState(state), transitions(otherStates), layers(layers) {}

  virtual ~GameScreen() {}

  // If we care about key input, then return whether or not the given key has
  // caused us to change states.
  virtual GameState handleInput(structures::InputState &state) {
    return myState;
  }

  // If text is supposed to be displayed on the screen, then pass it along.
  // This will be called for each frame.
  virtual void GetText(std::vector<structures::textInfo> &text) {
    text = screenText;
  }

#if LEGACY
  // If we care about key input, then return whether or not the given key has
  // caused us to change states.
  virtual GameState handleKeyInput(int key, bool pressed) { return myState; }

  // If we care about mouse input, then return whether or not the givem mouse
  // event has caused us to change states.
  virtual GameState handleMouseInput(int x, int y, bool pressed) {
    return myState;
  }
#endif // LEGACY

  virtual void update(unsigned int *buf, int width, int height) = 0;
};

} // namespace impl
#endif // _GAME_SCREEN_H_