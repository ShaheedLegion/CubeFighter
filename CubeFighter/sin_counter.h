#ifndef _SIN_COUNTER_H_
#define _SIN_COUNTER_H_
#pragma once
#include "singleton.h"
#include <math.h>

namespace detail {

class SinCounter : public Singleton<SinCounter> {
  int current;

  SinCounter() {
    current = 0;
    sins = new float[360];
    coss = new float[360];

    for (int i = 0; i < 360; ++i) {
      sins[i] = sin(static_cast<float>(i));
      coss[i] = cos(static_cast<float>(i));
    }
  }
  ~SinCounter() {}

  friend class detail::Singleton<SinCounter>;

  float *sins;
  float *coss;

public:
  void Tick() {
    ++current;
    if (current > 359)
      current = 0;
  }

  float getSin() const { return sins[current]; }
  float getCos() const { return coss[current]; }
};

} // namespace detail

#endif  // _SIN_COUNTER_H_