#pragma once
#include <lvgl.h>
#include <stdint.h>

class Arduino_DataBus;
class Arduino_GFX;

namespace board {

/**
 * The QSPI panel, and LVGL's view of it.
 *
 * Arduino_GFX keeps the bus and the AXS15231B init that already worked; this
 * class only hands LVGL's finished stripes to it. Nothing here runs in the
 * constructor — on Arduino, globals are constructed before Serial and before
 * the heap settles, and a failure there looks like a hardware fault.
 */
class Panel35 {
 public:
  Panel35() = default;
  Panel35(const Panel35&) = delete;
  Panel35& operator=(const Panel35&) = delete;

  bool begin();
  void set_backlight(uint8_t level);

  /** True when both draw buffers were allocated; one buffer means tearing. */
  bool double_buffered() const { return buffers_[1] != nullptr; }

 private:
  static void flush_trampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);
  void flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);

  Arduino_DataBus* bus_ = nullptr;
  Arduino_GFX* gfx_ = nullptr;
  lv_display_t* lv_display_ = nullptr;
  uint8_t* buffers_[2] = {nullptr, nullptr};
};

}  // namespace board
