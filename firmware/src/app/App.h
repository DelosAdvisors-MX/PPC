#pragma once
#include "../hal/Display.h"
#include "../hal/TouchPanel.h"
#include "../ui/Deck.h"

/**
 * The whole panel, owned in one place.
 *
 * Instantiated as a global, so every member's constructor must stay trivial:
 * on Arduino, globals are constructed before Serial exists and before the
 * heap has settled. All the real work happens in begin().
 */
class App {
 public:
  App() = default;
  App(const App&) = delete;
  App& operator=(const App&) = delete;

  /** Brings up the panel and starts the LVGL task. Call once from setup(). */
  void begin();

 private:
  static void task_trampoline(void* self);
  void run_lvgl();

  hal::Display display_;
  hal::TouchPanel touch_;
  ui::Deck deck_;
};
