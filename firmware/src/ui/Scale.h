#pragma once
#include <math.h>

namespace ui {

/** Rounds a rough step up to something a reader can count in: 1, 2, 2.5, 5... */
inline float nice_step(float rough) {
  const float magnitude = powf(10.0f, floorf(log10f(rough)));
  for (const float factor : {1.0f, 2.0f, 2.5f, 5.0f, 10.0f}) {
    if (factor * magnitude >= rough) return factor * magnitude;
  }
  return 10.0f * magnitude;
}

struct Ticks {
  float max;
  float step;
  int count;  // including zero
};

/**
 * An axis that starts at zero and ends on a round number above the data.
 *
 * Two months of similar size land on the same axis, so flicking between them
 * compares like with like instead of the chart silently rescaling underneath.
 */
inline Ticks zero_based_ticks(float peak, int intervals = 3) {
  if (!(peak > 0.0f)) return {1.0f, 1.0f, 2};
  const float step = nice_step(peak / intervals);
  const float max = ceilf(peak / step) * step;
  return {max, step, static_cast<int>(lroundf(max / step)) + 1};
}

}  // namespace ui
