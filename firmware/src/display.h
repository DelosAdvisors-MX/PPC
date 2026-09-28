#pragma once
#include <lvgl.h>

namespace display {

// Brings up the QSPI bus, the AXS15231B, the backlight and LVGL's display
// object. Call once from setup(), before anything touches lv_*.
void begin();

// 0-255. The panel is readable at about 60 indoors.
void set_backlight(uint8_t level);

}  // namespace display
