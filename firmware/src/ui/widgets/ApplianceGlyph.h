#pragma once
#include "../../data/MeterData.h"
#include "ChartWidget.h"

namespace ui {

/**
 * The line icon beside each appliance.
 *
 * Drawn from primitives rather than carried as an image: five icons at two
 * panel sizes would be ten bitmaps in a 4MB flash, and these are simple enough
 * that rectangles, circles and lines say them.
 */
class ApplianceGlyph : public ChartWidget {
 public:
  void set_icon(ApplianceIcon icon);

 protected:
  void draw(lv_layer_t* layer) override;

 private:
  ApplianceIcon icon_ = ApplianceIcon::HotWater;
};

}  // namespace ui
