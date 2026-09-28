#pragma once
#include <lvgl.h>

#include "../data/MeterData.h"

namespace ui {

/**
 * One screen of the deck.
 *
 * Screens are built once into their tile and then updated in place, so a new
 * reading re-labels what is already on screen rather than rebuilding it. They
 * are owned by the Deck as members, which keeps them off the heap and
 * guarantees they outlive the LVGL objects that point back at them.
 */
class Screen {
 public:
  virtual ~Screen() = default;
  Screen(const Screen&) = delete;
  Screen& operator=(const Screen&) = delete;

  /** Builds the screen into `tile`. Call once, with the LVGL lock held. */
  void mount(lv_obj_t* tile);

  /** Re-labels the screen from a new reading. Safe to call before mount(). */
  virtual void update(const MeterSnapshot& snapshot) = 0;

  virtual const char* name() const = 0;

 protected:
  Screen() = default;
  virtual void build(lv_obj_t* root) = 0;

  lv_obj_t* root_ = nullptr;
};

}  // namespace ui
