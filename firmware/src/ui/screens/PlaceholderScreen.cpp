#include "PlaceholderScreen.h"

#include "../Fonts.h"
#include "../Theme.h"

namespace ui {

void PlaceholderScreen::build(lv_obj_t* root) {
  lv_obj_t* card = lv_obj_create(root);
  lv_obj_remove_style_all(card);
  lv_obj_set_pos(card, theme::PANEL_PADDING, theme::PANEL_PADDING);
  lv_obj_set_size(card, theme::PANEL_WIDTH - 2 * theme::PANEL_PADDING, 54);
  lv_obj_set_style_bg_color(card, lv_color_hex(accent_), 0);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
  lv_obj_set_style_radius(card, 12, 0);
  lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t* label = lv_label_create(card);
  lv_label_set_text(label, name_);
  lv_obj_set_style_text_color(label, lv_color_hex(theme::SURFACE), 0);
  lv_obj_set_style_text_font(label, fonts::subtitle(), 0);
  lv_obj_center(label);
}

}  // namespace ui
