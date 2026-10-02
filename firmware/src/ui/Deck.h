#pragma once
#include <lvgl.h>

#include "Screen.h"
#include "screens/HomeScreen.h"
#include "screens/PlaceholderScreen.h"

namespace ui {

/**
 * The screen map, as an lv_tileview.
 *
 *   row 0                | P1 telegram |              |             |
 *   row 1  | Appliances  | Home        | Medium · Oct | High · 2026 | Compare
 *   row 2  |             | Home · Sep  | Medium · Sep | High · 2025 | Compare · Sep
 *
 * Tileview scrolls naturally — a finger moving up reveals the row below — so
 * the previous-period row sits under the current one and the telegram above
 * it. That is upside down against the design's diagram but right side up
 * against its legend: swipe up is still one period back. Rows cannot be
 * negative, which is the other reason the map is shifted down by one.
 *
 * KNOWN GAP: tileview's horizontal gesture is the opposite of what the design
 * asks for. Dragging left reveals the tile on the right; the web app reveals
 * the one on the left. Fixing it means taking the gesture off tileview and
 * driving lv_obj_set_tile_id from our own handler — see firmware/README.md.
 */
class Deck {
 public:
  Deck() = default;
  Deck(const Deck&) = delete;
  Deck& operator=(const Deck&) = delete;

  /** Builds every tile. Call once, with the LVGL lock held. */
  void build(lv_obj_t* parent);

  /** Pushes a new reading into every screen. Call with the LVGL lock held. */
  void update(const MeterSnapshot& snapshot);

 private:
  struct Slot {
    Screen* screen;
    uint8_t column;
    uint8_t row;
    lv_dir_t directions;
  };

  lv_obj_t* tileview_ = nullptr;

  HomeScreen home_{HomeScreen::Period::Current};
  HomeScreen home_previous_{HomeScreen::Period::Previous};
  PlaceholderScreen telegram_{"P1 telegram", 0x12161C};
  PlaceholderScreen appliances_{"Appliances", 0x2E6FD0};
  PlaceholderScreen medium_{"Medium - October", 0x2E6FD0};
  PlaceholderScreen medium_previous_{"Medium - September", 0xD6006E};
  PlaceholderScreen high_{"High - 2026", 0x2E6FD0};
  PlaceholderScreen high_previous_{"High - 2025", 0xD6006E};
  PlaceholderScreen compare_{"Compare", 0x2E6FD0};
  PlaceholderScreen compare_previous_{"Compare - September", 0xD6006E};
};

}  // namespace ui
