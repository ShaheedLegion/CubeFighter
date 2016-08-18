#pragma once

#include "resource.h"
#include "renderer.h"
#include "impl.h"
#include "structures.h"

#ifndef QUAD_RENDERER_HPP
#define QUAD_RENDERER_HPP

// typedef void (*RenderCall)(void);

namespace detail {

class QuadRenderer : public Renderer {
  bool running;
  impl::Game game;

public:
  QuadRenderer() : Renderer(), running(false) {}

  virtual void GetInputState(structures::InputState &state) override;
  virtual void Start() override;
  virtual void Stop() override;

  bool getRunning() const { return running; }

  void GetOutput(std::vector<structures::textInfo> &text) {
    game.GetOutput(text);
  }

  void Update(structures::InputState &state) {
    game.Update(state, getBuffer(), getW(), getH());
  }
};

} // namespace detail
#endif // QUAD_RENDERER_HPP