#include "App.h"

#include <Arduino.h>
#include <lvgl.h>

#include "../data/SampleData.h"

void App::task_trampoline(void* self) {
  static_cast<App*>(self)->run_lvgl();
}

/**
 * LVGL runs here and nowhere else.
 *
 * LVGL is not thread safe. Anything that wants to touch lv_* from another
 * task — a WiFi poll, a P1 parser — must take lv_lock() first, which is real
 * only because lv_conf.h sets LV_USE_OS to LV_OS_FREERTOS. Calling lv_* from
 * two tasks without it produces exactly the half-drawn, overlapping frames
 * the Arduino_GFX build showed.
 */
void App::run_lvgl() {
  for (;;) {
    lv_lock();
    uint32_t next_ms = lv_timer_handler();
    lv_unlock();

    if (next_ms == LV_NO_TIMER_READY || next_ms > 10) next_ms = 10;
    vTaskDelay(pdMS_TO_TICKS(next_ms == 0 ? 1 : next_ms));
  }
}

void App::begin() {
  Serial.begin(115200);
  delay(200);
  Serial.printf("\nPPC Smart Meter Display - %s\n", board::TARGET);

  lv_init();
  lv_tick_set_cb([]() -> uint32_t { return millis(); });

  if (!panel_.begin()) {
    Serial.println("App: panel did not come up");
    return;
  }
  touch_.begin();

#if defined(RGB_TEST_PATTERN)
  // Bring-up build: colour bars instead of the UI, to settle the pin mapping
  // and the panel timing before any of the design is in the way.
  panel_.draw_test_pattern();
  Serial.println("App: RGB test pattern - see README, Validating the RGB mapping");
  return;
#endif

  lv_lock();
  deck_.build(lv_screen_active());
  deck_.update(sample::kSnapshot);
  lv_unlock();

  // Pinned to core 1 so the WiFi stack on core 0 cannot stall a redraw.
  xTaskCreatePinnedToCore(task_trampoline, "lvgl", 8192, this, 2, nullptr, 1);

  Serial.printf("App: up on %s\n", board::TARGET);
}
