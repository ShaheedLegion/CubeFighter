#pragma once

#ifndef _RENDERER_HPP
#define _RENDERER_HPP

#include <vector>
#include "structures.h"

namespace detail {

class Renderer {
  structures::RenderCall renderCallback;
  unsigned int *buffer;
  int screenW;
  int screenH;

public:
  Renderer()
      : renderCallback(nullptr), buffer(nullptr), screenW(0), screenH(0) {}

  void SetRenderCallback(structures::RenderCall call) {
    renderCallback = call;
    // renderCallback();
  }

  void SetRenderInfo(unsigned int *bits, int w, int h) {
    buffer = bits;
    screenW = w;
    screenH = h;
  }

  void Paint(std::vector<structures::textInfo> &text) {
    if (renderCallback)
      renderCallback(text);
  }

  unsigned int *getBuffer() { return buffer; }
  int getW() const { return screenW; }
  int getH() const { return screenH; }

  // These must be handled by the derived classes.
  virtual void GetInputState(structures::InputState &state) = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
};

}
#endif // _RENDERER_HPP