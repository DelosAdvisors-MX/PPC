#include "Deck.h"

#include "../board/Metrics.h"
#include "Theme.h"

namespace ui {
namespace {

enum Column : uint8_t { APPLIANCES = 0, HOME, MEDIUM, HIGH, COMPARE };
enum Row : uint8_t { RAW = 0, CURRENT, PREVIOUS };

constexpr lv_dir_t HORIZONTAL_AND_DOWN = static_cast<lv_dir_t>(LV_DIR_HOR | LV_DIR_BOTTOM);
constexpr lv_dir_t UP_AND_HORIZONTAL = static_cast<lv_dir_t>(LV_DIR_TOP | LV_DIR_HOR);

}  // namespace

void Deck::build(lv_obj_t* parent) {
  tileview_ = lv_tileview_create(parent);
  lv_obj_set_size(tileview_, metrics::WIDTH, metrics::HEIGHT);
  lv_obj_set_style_bg_color(tileview_, lv_color_hex(theme::SURFACE), 0);
  lv_obj_remove_flag(tileview_, LV_OBJ_FLAG_SCROLL_ELASTIC);

  const Slot slots[] = {
      {&telegram_, HOME, RAW, LV_DIR_BOTTOM},

      {&appliances_, APPLIANCES, CURRENT, LV_DIR_RIGHT},
      {&home_, HOME, CURRENT, LV_DIR_ALL},
      {&medium_, MEDIUM, CURRENT, HORIZONTAL_AND_DOWN},
      {&high_, HIGH, CURRENT, HORIZONTAL_AND_DOWN},
      {&compare_, COMPARE, CURRENT, static_cast<lv_dir_t>(LV_DIR_LEFT | LV_DIR_BOTTOM)},

      // No n-1 view for appliances, so column 0 has no tile on this row.
      {&home_previous_, HOME, PREVIOUS, static_cast<lv_dir_t>(LV_DIR_TOP | LV_DIR_RIGHT)},
      {&medium_previous_, MEDIUM, PREVIOUS, UP_AND_HORIZONTAL},
      {&high_previous_, HIGH, PREVIOUS, UP_AND_HORIZONTAL},
      {&compare_previous_, COMPARE, PREVIOUS, static_cast<lv_dir_t>(LV_DIR_TOP | LV_DIR_LEFT)},
  };

  for (const Slot& slot : slots) {
    slot.screen->mount(lv_tileview_add_tile(tileview_, slot.column, slot.row, slot.directions));
  }

  lv_obj_set_tile_id(tileview_, HOME, CURRENT, LV_ANIM_OFF);
}

void Deck::update(const MeterSnapshot& snapshot) {
  home_.update(snapshot);
  home_previous_.update(snapshot);
}

}  // namespace ui
