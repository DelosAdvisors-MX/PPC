#pragma once
#include "ChartWidget.h"

namespace ui {

/**
 * The half dial on the Home screen.
 *
 * The arc takes the colour of the load band the reading falls in — green
 * under 2 kW, amber to 3,5, red above — so the dial is readable across a room
 * before the number is. The track stays neutral: colouring it too would leave
 * the arc competing with its own background.
 */
class Gauge : public ChartWidget {
 public:
  void set_reading(float value, float max);

 protected:
  void draw(lv_layer_t* layer) override;

 private:
  float value_ = 0.0f;
  float max_ = 5.0f;
};

}  // namespace ui
