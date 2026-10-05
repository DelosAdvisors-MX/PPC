#pragma once
#include <lvgl.h>

namespace ui {

/**
 * Base for the five drawn charts: gauge, fill bar, daily bars, year line,
 * compare bars.
 *
 * Each is one LVGL object whose main draw is taken over by a virtual method,
 * so nothing is buffered and nothing is composed from sub-objects. That maps
 * the web app's inline SVG components across almost line for line — the same
 * geometry, the same constants, different draw calls.
 *
 * Widgets must outlive the object they attach to, so screens hold them by
 * value rather than creating them on the stack.
 */
class ChartWidget {
 public:
  virtual ~ChartWidget();
  ChartWidget(const ChartWidget&) = delete;
  ChartWidget& operator=(const ChartWidget&) = delete;

  /** Creates the LVGL object and takes over its drawing. */
  void attach(lv_obj_t* parent, int32_t x, int32_t y, int32_t w, int32_t h);

  /** Ask for a repaint after the data behind the widget changes. */
  void invalidate();

  /** The LVGL object, so a screen can show or hide the whole widget. */
  lv_obj_t* obj() const { return obj_; }

  /** Drawing size, in device pixels. Charts lay themselves out from this. */
  int32_t width() const { return width_; }
  int32_t height() const { return height_; }

 protected:
  ChartWidget() = default;

  /**
   * Draw the widget. Coordinates are widget-local; `origin` is where the
   * widget sits on screen and is already added for you by local_x/local_y.
   */
  virtual void draw(lv_layer_t* layer) = 0;

  int32_t local_x(int32_t x) const { return origin_.x + x; }
  int32_t local_y(int32_t y) const { return origin_.y + y; }

  lv_obj_t* obj_ = nullptr;
  int32_t width_ = 0;
  int32_t height_ = 0;

 private:
  static void event_trampoline(lv_event_t* event);

  lv_point_t origin_ = {0, 0};
};

}  // namespace ui
