#pragma once
#include <lvgl.h>

namespace ui {

/**
 * The bordered card the Medium, High and Compare charts sit in. Padded, so a
 * child placed at (0, 0) lands inside the border rather than on it.
 */
lv_obj_t* make_card(lv_obj_t* parent, int32_t y, int32_t width = 0, int32_t x = 0);

/**
 * The small uppercase caption used under charts and beside units.
 *
 * Bold and near-black rather than grey: at this size on a panel held at arm's
 * length, grey-on-white is the first thing to disappear.
 */
lv_obj_t* make_eyebrow(lv_obj_t* parent, int32_t x, int32_t y);

}  // namespace ui
