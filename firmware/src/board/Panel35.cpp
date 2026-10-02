#include "Panel35.h"

#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <esp_heap_caps.h>

#include "Metrics.h"
#include "pins_35.h"

namespace board {
namespace {

/**
 * Two partial buffers, not one.
 *
 * With a single buffer LVGL must wait for each stripe to reach the panel
 * before drawing the next, so a swipe tears and screens appear to overlap.
 * With two it draws into one while the other is in flight.
 *
 * 40 rows is comfortably above LVGL's 1/10-of-screen guidance (32 rows here)
 * and costs 38 KB each. They must be DMA-capable internal RAM: a buffer in
 * PSRAM is slow and, on some configurations, not DMA-addressable at all.
 */
constexpr int BUFFER_ROWS = 40;
constexpr size_t BUFFER_BYTES = pins35::SCREEN_WIDTH * BUFFER_ROWS * sizeof(uint16_t);

}  // namespace

void Panel35::flush_trampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
  static_cast<Panel35*>(lv_display_get_user_data(disp))->flush(disp, area, px_map);
}

void Panel35::flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
  const int32_t w = area->x2 - area->x1 + 1;
  const int32_t h = area->y2 - area->y1 + 1;

  // LVGL renders RGB565 little-endian; the panel reads it the other way round.
  // Swapping here keeps LVGL's fast path and costs one pass over the stripe.
  // If colours come out inverted, this is the line to try removing.
  lv_draw_sw_rgb565_swap(px_map, w * h);

  gfx_->draw16bitRGBBitmap(area->x1, area->y1, reinterpret_cast<uint16_t*>(px_map), w, h);
  lv_display_flush_ready(disp);
}

void Panel35::set_backlight(uint8_t level) {
  analogWrite(pins35::LCD_BACKLIGHT, level);
}

bool Panel35::begin() {
  bus_ = new Arduino_ESP32QSPI(pins35::LCD_CS, pins35::LCD_CLK, pins35::LCD_D0,
                               pins35::LCD_D1, pins35::LCD_D2, pins35::LCD_D3);
  gfx_ = new Arduino_AXS15231B(bus_, pins35::LCD_RST, pins35::ROTATION,
                               /*ips=*/true, pins35::PANEL_NATIVE_WIDTH,
                               pins35::PANEL_NATIVE_HEIGHT);

  if (!gfx_->begin()) {
    Serial.println("Panel35: gfx->begin() failed");
    return false;
  }
  gfx_->fillScreen(BLACK);

  pinMode(pins35::LCD_BACKLIGHT, OUTPUT);
  set_backlight(200);

  for (uint8_t* & buffer : buffers_) {
    buffer = static_cast<uint8_t*>(
        heap_caps_malloc(BUFFER_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
  }
  if (!double_buffered()) {
    // One buffer still draws, but the tearing comes back. Say so here rather
    // than leaving it to be rediscovered from a video of the panel.
    Serial.println("Panel35: only one DMA buffer available - expect tearing");
  }

  lv_display_ = lv_display_create(pins35::SCREEN_WIDTH, pins35::SCREEN_HEIGHT);
  lv_display_set_user_data(lv_display_, this);
  lv_display_set_flush_cb(lv_display_, flush_trampoline);
  lv_display_set_buffers(lv_display_, buffers_[0], buffers_[1], BUFFER_BYTES,
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
  return true;
}

}  // namespace board
