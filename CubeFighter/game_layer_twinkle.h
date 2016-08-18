#ifndef GAME_LAYERS_H_TWINKLE
#define GAME_LAYERS_H_TWINKLE

#include "layers.h"

namespace impl {
class GameLayerTwinkle : public GameLayer {
  std::vector<structures::VERTEX> twinkles;

public:
  GameLayerTwinkle() {}
  virtual ~GameLayerTwinkle() {}

  // Handle input if needed, useful for stuff like parralax scrolling.
  virtual void handleInput(structures::InputState &state) override {}

  // Every layer must implement an update function.
  virtual void update(unsigned int *buf, int width, int height) override {}
};
} // namespace impl

#endif // GAME_LAYERS_H_TWINKLE