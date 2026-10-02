#pragma once
#include <esp_lcd_panel_ops.h>
#include <lvgl.h>
#include <stdint.h>

namespace board {

/**
 * The 7 inch CrowPanel: a 16-bit RGB panel driven straight by the ESP32-S3's
 * LCD_CAM peripheral, with its framebuffers in PSRAM.
 *
 * This does not go through Arduino_GFX. An RGB panel is not a bus you push
 * pixels down — the peripheral streams the framebuffer out continuously, for
 * ever, and the only things that matter are whether that stream keeps up and
 * how the UI gets its pixels into the buffer. Those two knobs live in the
 * ESP-IDF RGB driver and nowhere else, so that is what this talks to.
 */
class Panel70 {
 public:
  Panel70() = default;
  Panel70(const Panel70&) = delete;
  Panel70& operator=(const Panel70&) = delete;

  bool begin();
  void set_backlight(uint8_t level);

  /** Draws colour bars instead of the UI, to settle the RGB pin mapping. */
  void draw_test_pattern();

 private:
  static void flush_trampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);
  void flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map);

  esp_lcd_panel_handle_t panel_ = nullptr;
  lv_display_t* lv_display_ = nullptr;
  void* framebuffers_[2] = {nullptr, nullptr};
};

}  // namespace board
