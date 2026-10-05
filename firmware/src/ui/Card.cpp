#include "Card.h"

#include "../board/Metrics.h"
#include "Fonts.h"
#include "Theme.h"

namespace ui {

lv_obj_t* make_card(lv_obj_t* parent, int32_t y, int32_t width, int32_t x) {
  lv_obj_t* card = lv_obj_create(parent);
  lv_obj_remove_style_all(card);
  lv_obj_set_pos(card, x > 0 ? x : metrics::PADDING,  y);
  lv_obj_set_size(card, width > 0 ? width : metrics::CONTENT_WIDTH,
                  metrics::HEIGHT - y - metrics::PADDING);
  lv_obj_set_style_radius(card, metrics::px(12), 0);
  lv_obj_set_style_border_color(card, lv_color_hex(theme::BORDER), 0);
  lv_obj_set_style_border_width(card, 1, 0);
  lv_obj_set_style_border_opa(card, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_opa(card, LV_OPA_TRANSP, 0);
  lv_obj_set_style_pad_all(card, metrics::px(12), 0);
  lv_obj_remove_flag(card, LV_OBJ_FLAG_SCROLLABLE);
  return card;
}

lv_obj_t* make_eyebrow(lv_obj_t* parent, int32_t x, int32_t y) {
  lv_obj_t* label = lv_label_create(parent);
  lv_obj_set_pos(label, x, y);
  lv_obj_set_style_text_font(label, fonts::caption(), 0);
  lv_obj_set_style_text_color(label, lv_color_hex(theme::INK), 0);
  lv_obj_set_style_text_letter_space(label, 1, 0);
  lv_label_set_text(label, "");
  return label;
}

}  // namespace ui
