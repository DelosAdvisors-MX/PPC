#pragma once
#include <lvgl.h>

namespace ui {

// Builds the screen map on the active LVGL screen. Call with the LVGL lock held.
void build_deck();

}  // namespace ui
