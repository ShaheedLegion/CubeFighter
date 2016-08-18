#ifndef _GAME_SCREEN_START_H_
#define _GAME_SCREEN_START_H_
#pragma once

#include "game_state.h"
#include "texture_loader.h"
#include <vector>

namespace impl {

class GameScreenStart : public GameScreen {
  detail::Texture *logo;

public:
  GameScreenStart(GameState state, const std::vector<GameState> &otherStates,
                  const std::vector<GameLayer *> &layers)
      : GameScreen(state, otherStates, layers) {
    // Allocate all the resources, set the state.
    detail::TextureLoader *loader = detail::TextureLoader::getInstance();
    logo = &loader->getTexture("logo.graw");

    structures::textInfo info(100, 100, 0xffffff, true, 500,
                              "Hit spacebar to start.");
    screenText.push_back(info);
  }

  virtual ~GameScreenStart() {}

  virtual GameState handleInput(structures::InputState &state) override {
    for (auto layer : layers) {
      layer->handleInput(state);
    }
    bool keyPressed = false;
    for (int i = 0; i < state.kpressedLength; ++i) {
      if (state.kpressed[i]) {
        keyPressed = true;
        break;
      }
    }

    if (keyPressed) {
      state.reset();
      return (transitions.size() ? transitions[0] : myState);
    }

    return myState;
  }

  virtual void update(unsigned int *buf, int w, int h) override {
    // We can now render our thing onto the buffer ...
    for (auto layer : layers) {
      layer->update(buf, w, h);
    }
    // So, we need to perturb this buffer by some value ....
    detail::SinCounter *counter = detail::SinCounter::getInstance();
    structures::ARGB *dst = reinterpret_cast<structures::ARGB *>(buf);
    structures::ARGB *src =
        reinterpret_cast<structures::ARGB *>(logo->getBuffer());
    int len = w * h;
    int idx = len;

    while (--len > 0) {
      counter->Tick();
      float sin = counter->getSin();
      float cos = counter->getCos();
      structures::ARGB tmp = *src;
      tmp.b += static_cast<int>(sin * 10);

      *dst = tmp;

      ++dst;
      ++src;
    }
  }
};

} // namespace impl
#endif // _GAME_SCREEN_START_H_