#include "ChartWidget.h"

namespace ui {

ChartWidget::~ChartWidget() {
  // The screen owning this widget normally outlives the tile, but if an object
  // is deleted first LVGL would still hold a pointer to us.
  if (obj_ != nullptr) lv_obj_remove_event_cb(obj_, event_trampoline);
}

void ChartWidget::event_trampoline(lv_event_t* event) {
  auto* self = static_cast<ChartWidget*>(lv_event_get_user_data(event));
  lv_obj_t* obj = static_cast<lv_obj_t*>(lv_event_get_target(event));

  lv_area_t coords;
  lv_obj_get_coords(obj, &coords);
  self->origin_ = {coords.x1, coords.y1};

  self->draw(lv_event_get_layer(event));
}

void ChartWidget::attach(lv_obj_t* parent, int32_t x, int32_t y, int32_t w, int32_t h) {
  obj_ = lv_obj_create(parent);
  lv_obj_remove_style_all(obj_);
  lv_obj_set_pos(obj_, x, y);
  lv_obj_set_size(obj_, w, h);
  lv_obj_remove_flag(obj_, LV_OBJ_FLAG_SCROLLABLE);
  // The deck scrolls; a widget that eats the gesture would trap the swipe.
  lv_obj_add_flag(obj_, LV_OBJ_FLAG_EVENT_BUBBLE);
  lv_obj_add_event_cb(obj_, event_trampoline, LV_EVENT_DRAW_MAIN, this);
}

void ChartWidget::invalidate() {
  if (obj_ != nullptr) lv_obj_invalidate(obj_);
}

}  // namespace ui
