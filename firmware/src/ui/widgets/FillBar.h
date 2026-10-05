#pragma once
#include "ChartWidget.h"

namespace ui {

/**
 * How far into the month's estimate we are, as a vertical bar against a
 * 0/50/100% scale. 182 x 206, as in the design.
 *
 * A closed month paints magenta and draws the 100% line over the bar; past
 * 100% the bar stops at the ceiling and the overshoot is told in the caption.
 */
class FillBar : public ChartWidget {
 public:
  void set_progress(float used_kwh, float estimate_kwh, bool closed);

 protected:
  void draw(lv_layer_t* layer) override;

 private:
  float used_ = 0.0f;
  float estimate_ = 1.0f;
  bool closed_ = false;
};

}  // namespace ui
