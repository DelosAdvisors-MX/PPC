#include "display.h"

#include <Arduino_GFX_Library.h>
#include <esp_heap_caps.h>

#include "board_config.h"

namespace display {
namespace {

Arduino_DataBus *bus = nullptr;
Arduino_GFX *gfx = nullptr;

/**
 * Two partial buffers, not one.
 *
 * With a single buffer LVGL has to wait for each flush to reach the panel
 * before it can draw the next stripe, so a swipe tears and screens appear to
 * overlap. With two it draws into one while the other is in flight.
 *
 * 40 rows is well above LVGL's 1/10-of-screen guidance (32 rows here) and
 * costs 38 KB each. They must be DMA-capable internal RAM: a buffer in PSRAM
 * is both slow and, on some configurations, not DMA-addressable at all.
 */
constexpr int BUFFER_ROWS = 40;
constexpr size_t BUFFER_PIXELS = board::SCREEN_WIDTH * BUFFER_ROWS;
constexpr size_t BUFFER_BYTES = BUFFER_PIXELS * sizeof(uint16_t);

void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  const int32_t w = area->x2 - area->x1 + 1;
  const int32_t h = area->y2 - area->y1 + 1;

  // LVGL renders RGB565 little-endian; the panel wants it the other way round.
  // Swapping here keeps LVGL's fast path intact and costs one pass over the
  // stripe. If colours come out inverted, this is the line to try removing.
  lv_draw_sw_rgb565_swap(px_map, w * h);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, reinterpret_cast<uint16_t *>(px_map), w, h);
  lv_display_flush_ready(disp);
}

}  // namespace

void set_backlight(uint8_t level) {
  analogWrite(board::LCD_BACKLIGHT, level);
}

void begin() {
  bus = new Arduino_ESP32QSPI(board::LCD_CS, board::LCD_CLK, board::LCD_D0,
                              board::LCD_D1, board::LCD_D2, board::LCD_D3);
  gfx = new Arduino_AXS15231B(bus, board::LCD_RST, board::ROTATION,
                              /*ips=*/true, board::PANEL_NATIVE_WIDTH,
                              board::PANEL_NATIVE_HEIGHT);

  if (!gfx->begin()) {
    Serial.println("display: gfx->begin() failed");
    return;
  }
  gfx->fillScreen(BLACK);

  pinMode(board::LCD_BACKLIGHT, OUTPUT);
  set_backlight(200);

  auto *buf1 = static_cast<uint8_t *>(
      heap_caps_malloc(BUFFER_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
  auto *buf2 = static_cast<uint8_t *>(
      heap_caps_malloc(BUFFER_BYTES, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL));
  if (buf1 == nullptr || buf2 == nullptr) {
    // Falling back to one buffer still draws, but the tearing comes back, so
    // say so loudly rather than leaving it to be rediscovered from a video.
    Serial.println("display: could not allocate two DMA buffers - expect tearing");
  }

  lv_display_t *disp = lv_display_create(board::SCREEN_WIDTH, board::SCREEN_HEIGHT);
  lv_display_set_flush_cb(disp, flush_cb);
  lv_display_set_buffers(disp, buf1, buf2, BUFFER_BYTES,
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
}

}  // namespace display
