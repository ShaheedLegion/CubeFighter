#ifndef GAME_LAYERS_H_STARS
#define GAME_LAYERS_H_STARS

#include "layers.h"

namespace impl {
class GameLayerStars : public GameLayer {
  std::vector<structures::VERTEX> stars;

public:
  GameLayerStars(int numStars) {
    // Assume an acceptable amount of "stars" will always be numStars.
    for (int i = 0; i < numStars; ++i) {
      stars.push_back(structures::VERTEX());
    }
  }
  virtual ~GameLayerStars() {}

  // Handle input if needed, useful for stuff like parralax scrolling.
  virtual void handleInput(structures::InputState &state) override {}

  // Every layer must implement an update function.
  virtual void update(unsigned int *buf, int width, int height) override {}
};
} // namespace impl

#endif // GAME_LAYERS_H_STARS