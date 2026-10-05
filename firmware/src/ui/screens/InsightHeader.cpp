#include "InsightHeader.h"

#include "../../board/Metrics.h"
#include "../Fonts.h"
#include "../Theme.h"

namespace ui {

void InsightHeader::build(lv_obj_t* parent, int32_t y) {
  const int32_t height = metrics::px(54);
  const int32_t badge_w = metrics::px(150);
  const int32_t gap = metrics::px(10);
  const int32_t pill_w = metrics::CONTENT_WIDTH - badge_w - gap;

  lv_obj_t* pill = lv_obj_create(parent);
  lv_obj_remove_style_all(pill);
  lv_obj_set_pos(pill, metrics::PADDING, y);
  lv_obj_set_size(pill, pill_w, height);
  lv_obj_set_style_radius(pill, metrics::px(12), 0);
  lv_obj_set_style_bg_opa(pill, LV_OPA_COVER, 0);
  // LVGL 8 can only run a gradient along an axis, so the design's 120 degrees
  // becomes horizontal. At this size the difference is not visible.
  lv_obj_set_style_bg_color(pill, lv_color_hex(theme::INSIGHT_GRADIENT_FROM), 0);
  lv_obj_set_style_bg_grad_color(pill, lv_color_hex(theme::INSIGHT_GRADIENT_TO), 0);
  lv_obj_set_style_bg_grad_dir(pill, LV_GRAD_DIR_HOR, 0);
  lv_obj_remove_flag(pill, LV_OBJ_FLAG_SCROLLABLE);

  title_ = lv_label_create(pill);
  lv_obj_align(title_, LV_ALIGN_LEFT_MID, metrics::px(16), 0);
  lv_obj_set_style_text_font(title_, fonts::subtitle(), 0);
  lv_obj_set_style_text_color(title_, lv_color_hex(theme::SURFACE), 0);

  lv_obj_t* badge = lv_obj_create(parent);
  lv_obj_remove_style_all(badge);
  lv_obj_set_pos(badge, metrics::PADDING + pill_w + gap, y);
  lv_obj_set_size(badge, badge_w, height);
  lv_obj_set_style_radius(badge, metrics::px(12), 0);
  lv_obj_set_style_bg_opa(badge, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(badge, lv_color_hex(theme::AMBER), 0);
  lv_obj_remove_flag(badge, LV_OBJ_FLAG_SCROLLABLE);

  stat_ = lv_label_create(badge);
  lv_obj_align(stat_, LV_ALIGN_TOP_LEFT, metrics::px(14), metrics::px(8));
  lv_obj_set_style_text_font(stat_, fonts::title(), 0);
  lv_obj_set_style_text_color(stat_, lv_color_hex(theme::AMBER_INK), 0);

  stat_label_ = lv_label_create(badge);
  lv_obj_align(stat_label_, LV_ALIGN_BOTTOM_LEFT, metrics::px(14), -metrics::px(8));
  lv_obj_set_style_text_font(stat_label_, fonts::micro(), 0);
  lv_obj_set_style_text_color(stat_label_, lv_color_hex(theme::AMBER_INK), 0);
}

void InsightHeader::set(const char* title, const char* stat, const char* stat_label) {
  if (title_ == nullptr) return;
  lv_label_set_text(title_, title);
  lv_label_set_text(stat_, stat);
  lv_label_set_text(stat_label_, stat_label);
}

}  // namespace ui
