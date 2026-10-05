#pragma once
#include <stddef.h>

#include "ChartWidget.h"

namespace ui {

/**
 * Twelve months, January first — however many of them have happened.
 *
 * The axis is always a whole year and always starts at zero, so a year in
 * progress leaves the rest of it empty rather than stretching to fill it, and
 * two years can be compared without the chart rescaling underneath.
 */
class YearLine : public ChartWidget {
 public:
  void set_year(const float* monthly, size_t months);

 protected:
  void draw(lv_layer_t* layer) override;

 private:
  const float* monthly_ = nullptr;
  size_t months_ = 0;
};

}  // namespace ui
