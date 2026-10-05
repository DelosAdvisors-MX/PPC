#pragma once
#include <stddef.h>

#include "ChartWidget.h"

namespace ui {

/**
 * One bar per day, the peak in magenta, the month's average as a dashed rule.
 *
 * Bars are spaced across the whole month, not across the data, so five days
 * into October looks like five days into October rather than like a five-day
 * month. The y axis ends on a round number so two months share a scale.
 */
class DailyBars : public ChartWidget {
 public:
  void set_month(const float* daily, size_t days, size_t days_in_month);

 protected:
  void draw(lv_layer_t* layer) override;

 private:
  const float* daily_ = nullptr;
  size_t days_ = 0;
  size_t days_in_month_ = 31;
};

}  // namespace ui
