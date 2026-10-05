#pragma once
#include <lvgl.h>

namespace ui {

/**
 * The gradient title pill and amber average badge that head the Medium and
 * High screens. Built from styled objects rather than a draw callback: it is
 * boxes and text, and LVGL already does boxes and text.
 */
class InsightHeader {
 public:
  void build(lv_obj_t* parent, int32_t y);
  void set(const char* title, const char* stat, const char* stat_label);

 private:
  lv_obj_t* title_ = nullptr;
  lv_obj_t* stat_ = nullptr;
  lv_obj_t* stat_label_ = nullptr;
};

}  // namespace ui
