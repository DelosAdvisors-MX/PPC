#pragma once
#include <lvgl.h>

#include "Screen.h"
#include "screens/AppliancesScreen.h"
#include "screens/CompareScreen.h"
#include "screens/HighScreen.h"
#include "screens/HomeScreen.h"
#include "screens/MediumScreen.h"
#include "screens/TelegramScreen.h"

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
 * Every tile is a real screen. Each one is built once into its tile and then
 * updated in place, so a new reading re-labels what is already there rather
 * than rebuilding it.
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

  // Held by value: LVGL keeps raw pointers back to these, so they must outlive
  // their tiles.
  HomeScreen home_{HomeScreen::Period::Current};
  HomeScreen home_previous_{HomeScreen::Period::Previous};
  TelegramScreen telegram_;
  AppliancesScreen appliances_;
  MediumScreen medium_{MediumScreen::Period::Current};
  MediumScreen medium_previous_{MediumScreen::Period::Previous};
  HighScreen high_{HighScreen::Period::Current};
  HighScreen high_previous_{HighScreen::Period::Previous};
  CompareScreen compare_{CompareScreen::Period::Current};
  CompareScreen compare_previous_{CompareScreen::Period::Previous};

  /** Every screen, so update() cannot forget one. */
  Screen* all_[10] = {&home_,  &home_previous_,   &telegram_, &appliances_, &medium_,
                      &medium_previous_, &high_, &high_previous_, &compare_, &compare_previous_};
};

}  // namespace ui
