#ifndef GAME_LAYERS_H_
#define GAME_LAYERS_H_

#include "structures.h"

namespace impl {
	class GameLayer {
	public:
		GameLayer(){}
		virtual ~GameLayer() {}

		// Handle input if needed, useful for stuff like parralax scrolling.
		virtual void handleInput(structures::InputState &state) {}

		// Every layer must implement an update function.
		virtual void update(unsigned int *buf, int width, int height) = 0;
	};
}  // namespace impl

#endif  // GAME_LAYERS_H_