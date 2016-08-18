#ifndef _GAME_SCREEN_MENU_H_
#define _GAME_SCREEN_MENU_H_
#pragma once

#include "game_state.h"
#include "texture_loader.h"
#include <vector>

namespace impl {

class GameScreenMenu : public GameScreen {
  detail::Texture *logo;

public:
  GameScreenMenu(GameState state, const std::vector<GameState> &otherStates,
                 const std::vector<GameLayer *> &layers)
      : GameScreen(state, otherStates, layers) {
    // Allocate all the resources, set the state.
    detail::TextureLoader *loader = detail::TextureLoader::getInstance();
    logo = &loader->getTexture("menu.graw");
  }

  virtual ~GameScreenMenu() {}

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
      tmp.g += static_cast<int>(sin * 100);
      tmp.r += static_cast<int>(cos * (sin * 100));
      tmp.a += static_cast<int>((sin * 100) * cos);

      *dst = tmp;

      ++dst;
      ++src;
    }
  }
};

} // namespace impl
#endif // _GAME_SCREEN_MENU_H_