#pragma once
#include "ChartWidget.h"

namespace ui {

/**
 * The half dial on the Home screen: a grey track, a blue arc up to the
 * reading, and a needle. 240 x 152, as in the design.
 */
class Gauge : public ChartWidget {
 public:
  static constexpr int32_t WIDTH = 240;
  static constexpr int32_t HEIGHT = 152;

  void set_reading(float value, float max);

 protected:
  void draw(lv_layer_t* layer) override;

 private:
  float value_ = 0.0f;
  float max_ = 5.0f;
};

}  // namespace ui
