#ifndef _STRUCTS_TYPES_H
#define _STRUCTS_TYPES_H
#pragma once

#include <string>
#include <vector>

namespace structures {

#pragma pack(1)
struct ARGB {
  unsigned char b;
  unsigned char g;
  unsigned char r;
  unsigned char a;
};

// Vertices will be the basis of the game.
struct VERTEX {
  float x;
  float y;
  float z;
  float theta;
  int tx;
  int ty;
  ARGB col;

  VERTEX() : x(-1.0f), y(-1.0f), z(-1.0f), theta(-1.0f), tx(-1), ty(-1) {}
};

struct InputState {
  int mx, my;
  bool mpressed;
  const int kpressedLength = 7;
  bool kpressed[7]; // left, up, right, down, space, enter, escape

  void reset() {
    for (int i = 0; i < kpressedLength; ++i)
      kpressed[i] = false;

    mx = -1;
    my = -1;
    mpressed = false;
  }
};

struct textInfo {
  int x;
  int y;
  int col;

  bool flashing;
  int flashMillis;

  std::string text;

  textInfo(int x, int y, int col, bool flashing, int flashMillis,
           const std::string &text)
      : x(x), y(y), col(col), flashing(flashing), flashMillis(flashMillis),
        text(text) {}
};

typedef void (*RenderCall)(std::vector<textInfo> &text);

}; // namespace structures

#endif // _STRUCTS_TYPES_H