#pragma once
#include "../board/Board.h"
#include "../ui/Deck.h"

/**
 * The whole panel, owned in one place.
 *
 * Which panel is decided at compile time by board/Board.h, so this file is the
 * same for both targets.
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

  board::ActivePanel panel_;
  board::ActiveTouch touch_;
  ui::Deck deck_;
};
