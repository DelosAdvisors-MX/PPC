#include "Panel70.h"

#include <Arduino.h>
#include <esp_heap_caps.h>
#include <esp_lcd_panel_rgb.h>

#include "Metrics.h"
#include "pins_70.h"

namespace board {
namespace {

using namespace pins70;

/**
 * Bounce buffers are the fix for stripes down the left edge.
 *
 * The RGB peripheral reads the framebuffer continuously. With the framebuffer
 * in PSRAM — and 768 KB will not fit anywhere else — that read competes with
 * the CPU, the cache and WiFi for the same memory. Lose the race and the
 * peripheral's FIFO runs dry mid-line, the panel keeps clocking, and what it
 * latches is whatever was left on the bus. It shows up at the start of lines,
 * which is the left edge, and it drifts, which is what makes it look like
 * flicker rather than like a fixed artefact.
 *
 * With bounce buffers the DMA reads from internal SRAM instead, and a driver
 * task refills those from PSRAM in bursts. Bursts are what PSRAM is good at.
 *
 * Ten lines each, two of them: 800 x 10 x 2 bytes x 2 = 32 KB of internal RAM.
 * If stripes survive, raise this before touching anything else.
 */
constexpr int BOUNCE_LINES = 10;
constexpr size_t BOUNCE_BUFFER_PX = H_RES * BOUNCE_LINES;

constexpr size_t FRAME_BYTES = static_cast<size_t>(H_RES) * V_RES * 2;

}  // namespace

void Panel70::flush_trampoline(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
  static_cast<Panel70*>(lv_display_get_user_data(disp))->flush(disp, area, px_map);
}

/**
 * LVGL renders a whole frame into one of the panel's own framebuffers, so this
 * hands the driver a pointer rather than copying 768 KB across PSRAM. That copy
 * is the lag: at 15 MHz a frame is already 32 ms, and a full-frame memcpy
 * through PSRAM costs about as much again.
 */
void Panel70::flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
  esp_lcd_panel_draw_bitmap(panel_, area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map);
  lv_display_flush_ready(disp);
}

void Panel70::set_backlight(uint8_t level) {
  // 1.2 kHz: fast enough not to be seen, slow enough not to whine.
  ledcAttach(LCD_BACKLIGHT, 1200, 8);
  ledcWrite(LCD_BACKLIGHT, level);
}

bool Panel70::begin() {
  esp_lcd_rgb_panel_config_t config = {};
  config.clk_src = LCD_CLK_SRC_PLL160M;
  config.data_width = 16;
  config.bits_per_pixel = 16;
  // Two framebuffers, so a finished frame is shown by swapping rather than by
  // being copied over the one the panel is reading. No tearing, no copy.
  config.num_fbs = 2;
  config.bounce_buffer_size_px = BOUNCE_BUFFER_PX;
  config.psram_trans_align = 64;
  config.sram_trans_align = 4;

  config.hsync_gpio_num = LCD_HSYNC;
  config.vsync_gpio_num = LCD_VSYNC;
  config.de_gpio_num = LCD_DE;
  config.pclk_gpio_num = LCD_PCLK;
  config.disp_gpio_num = -1;

  const int data_pins[16] = {
      LCD_B0, LCD_B1, LCD_B2, LCD_B3, LCD_B4,
      LCD_G0, LCD_G1, LCD_G2, LCD_G3, LCD_G4, LCD_G5,
      LCD_R0, LCD_R1, LCD_R2, LCD_R3, LCD_R4,
  };
  for (int i = 0; i < 16; i++) config.data_gpio_nums[i] = data_pins[i];

  config.timings.pclk_hz = PCLK_HZ;
  config.timings.h_res = H_RES;
  config.timings.v_res = V_RES;
  config.timings.hsync_front_porch = HSYNC_FRONT_PORCH;
  config.timings.hsync_pulse_width = HSYNC_PULSE_WIDTH;
  config.timings.hsync_back_porch = HSYNC_BACK_PORCH;
  config.timings.vsync_front_porch = VSYNC_FRONT_PORCH;
  config.timings.vsync_pulse_width = VSYNC_PULSE_WIDTH;
  config.timings.vsync_back_porch = VSYNC_BACK_PORCH;
  config.timings.flags.pclk_active_neg = PCLK_ACTIVE_NEG;

  config.flags.fb_in_psram = true;

  if (esp_lcd_new_rgb_panel(&config, &panel_) != ESP_OK) {
    Serial.println("Panel70: esp_lcd_new_rgb_panel failed");
    return false;
  }
  if (esp_lcd_panel_reset(panel_) != ESP_OK || esp_lcd_panel_init(panel_) != ESP_OK) {
    Serial.println("Panel70: panel init failed");
    return false;
  }

  if (esp_lcd_rgb_panel_get_frame_buffer(panel_, 2, &framebuffers_[0], &framebuffers_[1]) !=
      ESP_OK) {
    Serial.println("Panel70: could not take both framebuffers");
    return false;
  }

  set_backlight(200);

  lv_display_ = lv_display_create(H_RES, V_RES);
  lv_display_set_user_data(lv_display_, this);
  lv_display_set_flush_cb(lv_display_, flush_trampoline);
  lv_display_set_color_format(lv_display_, LV_COLOR_FORMAT_RGB565);
  // FULL, not PARTIAL: LVGL draws straight into the panel's own buffers.
  lv_display_set_buffers(lv_display_, framebuffers_[0], framebuffers_[1], FRAME_BYTES,
                         LV_DISPLAY_RENDER_MODE_FULL);

  Serial.printf("Panel70: %dx%d @ %.1f MHz, refresh %.1f Hz, bounce %d lines\n", H_RES, V_RES,
                PCLK_HZ / 1e6f,
                static_cast<float>(PCLK_HZ) /
                    ((H_RES + HSYNC_FRONT_PORCH + HSYNC_PULSE_WIDTH + HSYNC_BACK_PORCH) *
                     (V_RES + VSYNC_FRONT_PORCH + VSYNC_PULSE_WIDTH + VSYNC_BACK_PORCH)),
                BOUNCE_LINES);
  return true;
}

/**
 * Colour bars and ramps, drawn straight into the framebuffer.
 *
 * Red, green and blue at full and at half, then a ramp per channel. If the pin
 * mapping is right the bars are pure and the ramps are smooth. A channel wired
 * in the wrong bit order gives a ramp that steps backwards or doubles back;
 * two channels swapped gives bars in the wrong order. Either is obvious in a
 * glance, which is the point — guessing at a pin map from a photo of the UI is
 * not.
 */
void Panel70::draw_test_pattern() {
  auto* fb = static_cast<uint16_t*>(framebuffers_[0]);
  if (fb == nullptr) return;

  const uint16_t bars[8] = {
      0xF800,  // red
      0x07E0,  // green
      0x001F,  // blue
      0xFFFF,  // white
      0x7800,  // half red
      0x03E0,  // half green
      0x000F,  // half blue
      0x0000,  // black
  };

  for (int y = 0; y < V_RES; y++) {
    uint16_t* row = fb + static_cast<size_t>(y) * H_RES;
    if (y < V_RES / 2) {
      for (int x = 0; x < H_RES; x++) row[x] = bars[(x * 8) / H_RES];
      continue;
    }
    // Lower half: a ramp per channel, one band each.
    const int band = (y - V_RES / 2) / ((V_RES / 2) / 3);
    for (int x = 0; x < H_RES; x++) {
      const int level = (x * 32) / H_RES;             // 0..31
      const int green = (x * 64) / H_RES;             // 0..63
      row[x] = band == 0   ? static_cast<uint16_t>(level << 11)
               : band == 1 ? static_cast<uint16_t>(green << 5)
                           : static_cast<uint16_t>(level);
    }
  }
  esp_lcd_panel_draw_bitmap(panel_, 0, 0, H_RES, V_RES, fb);
}

}  // namespace board
