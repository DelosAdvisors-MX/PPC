#include <Arduino.h>
#include <lvgl.h>

#include "board_config.h"
#include "display.h"
#include "touch.h"
#include "ui/deck.h"

namespace {

/**
 * LVGL runs in one task and nowhere else.
 *
 * LVGL is not thread safe. Anything that wants to touch lv_* from another
 * task — a WiFi poll, a P1 parser — must take lv_lock() first, which is real
 * only because lv_conf.h sets LV_USE_OS to LV_OS_FREERTOS. Calling lv_* from
 * two tasks without it produces exactly the half-drawn, overlapping frames
 * the Arduino_GFX build showed.
 */
void lvgl_task(void *) {
  for (;;) {
    lv_lock();
    uint32_t next_ms = lv_timer_handler();
    lv_unlock();

    if (next_ms == LV_NO_TIMER_READY || next_ms > 10) next_ms = 10;
    vTaskDelay(pdMS_TO_TICKS(next_ms ? next_ms : 1));
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);

  lv_init();
  lv_tick_set_cb([]() -> uint32_t { return millis(); });

  display::begin();
  touch::begin();

  lv_lock();
  ui::build_deck();
  lv_unlock();

  // Pinned to core 1 so the WiFi stack on core 0 cannot stall the redraw.
  xTaskCreatePinnedToCore(lvgl_task, "lvgl", 8192, nullptr, 2, nullptr, 1);

  Serial.println("ppc-panel: up");
}

void loop() {
  // Everything happens in tasks. Yield rather than spin.
  vTaskDelay(pdMS_TO_TICKS(1000));
}
