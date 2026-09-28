#include "Screen.h"

#include "Theme.h"

namespace ui {

void Screen::mount(lv_obj_t* tile) {
  root_ = tile;
  lv_obj_remove_flag(root_, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(root_, lv_color_hex(theme::SURFACE), 0);
  lv_obj_set_style_bg_opa(root_, LV_OPA_COVER, 0);
  lv_obj_set_style_pad_all(root_, 0, 0);
  build(root_);
}

}  // namespace ui
