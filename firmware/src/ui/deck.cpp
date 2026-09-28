#include "deck.h"

#include "../board_config.h"

namespace ui {
namespace {

/**
 * The screen map, as an lv_tileview.
 *
 *   row 0                | P1 telegram |              |             |
 *   row 1  | Appliances  | Home        | Medium · Aug | High · 2026 | Compare
 *   row 2  |             | Home · Aug  | Medium · Jul | High · 2025 | Compare · Aug
 *
 * Tileview scrolls naturally — a finger moving up reveals the row below — so
 * the previous-period row sits *under* the current one and the telegram sits
 * above it. That is upside down against the design's navigation map but right
 * side up against its legend: swipe up still means one period back, swipe
 * down from Home still reaches the telegram. Rows cannot be negative, which
 * is the other reason the whole map is shifted down by one.
 */
enum Col { COL_APPLIANCES = 0, COL_HOME, COL_MEDIUM, COL_HIGH, COL_COMPARE };
enum Row { ROW_RAW = 0, ROW_CURRENT, ROW_PREVIOUS };

// Placeholder until the real screens land. Proves the map and the gestures.
void add_placeholder(lv_obj_t *tile, const char *title, lv_color_t accent) {
  lv_obj_set_style_bg_color(tile, lv_color_hex(0xFFFFFF), 0);
  lv_obj_set_style_bg_opa(tile, LV_OPA_COVER, 0);

  lv_obj_t *bar = lv_obj_create(tile);
  lv_obj_set_size(bar, board::SCREEN_WIDTH - 32, 54);
  lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, 16);
  lv_obj_set_style_bg_color(bar, accent, 0);
  lv_obj_set_style_radius(bar, 12, 0);
  lv_obj_set_style_border_width(bar, 0, 0);

  lv_obj_t *label = lv_label_create(bar);
  lv_label_set_text(label, title);
  lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
  lv_obj_center(label);
}

}  // namespace

void build_deck() {
  lv_obj_t *tv = lv_tileview_create(lv_screen_active());
  lv_obj_set_style_bg_color(tv, lv_color_hex(0xFFFFFF), 0);
  lv_obj_remove_flag(tv, LV_OBJ_FLAG_SCROLL_ELASTIC);

  const lv_color_t blue = lv_color_hex(0x2E6FD0);
  const lv_color_t magenta = lv_color_hex(0xD6006E);
  const lv_color_t ink = lv_color_hex(0x12161C);

  // Raw row: the telegram hangs above Home and nothing else.
  add_placeholder(lv_tileview_add_tile(tv, COL_HOME, ROW_RAW, LV_DIR_BOTTOM),
                  "P1 telegram", ink);

  // Current row: complexity rises left to right.
  add_placeholder(lv_tileview_add_tile(tv, COL_APPLIANCES, ROW_CURRENT, LV_DIR_RIGHT),
                  "Appliances", blue);
  add_placeholder(lv_tileview_add_tile(tv, COL_HOME, ROW_CURRENT, LV_DIR_ALL),
                  "Home", blue);
  add_placeholder(lv_tileview_add_tile(tv, COL_MEDIUM, ROW_CURRENT,
                                       LV_DIR_HOR | LV_DIR_BOTTOM),
                  "Medium - August", blue);
  add_placeholder(lv_tileview_add_tile(tv, COL_HIGH, ROW_CURRENT,
                                       LV_DIR_HOR | LV_DIR_BOTTOM),
                  "High - 2026", blue);
  add_placeholder(lv_tileview_add_tile(tv, COL_COMPARE, ROW_CURRENT,
                                       LV_DIR_LEFT | LV_DIR_BOTTOM),
                  "Compare", magenta);

  // Previous row: no n-1 view for appliances, so column 0 is empty here.
  add_placeholder(lv_tileview_add_tile(tv, COL_HOME, ROW_PREVIOUS,
                                       LV_DIR_TOP | LV_DIR_RIGHT),
                  "Home - August", magenta);
  add_placeholder(lv_tileview_add_tile(tv, COL_MEDIUM, ROW_PREVIOUS,
                                       LV_DIR_TOP | LV_DIR_HOR),
                  "Medium - July", magenta);
  add_placeholder(lv_tileview_add_tile(tv, COL_HIGH, ROW_PREVIOUS,
                                       LV_DIR_TOP | LV_DIR_HOR),
                  "High - 2025", magenta);
  add_placeholder(lv_tileview_add_tile(tv, COL_COMPARE, ROW_PREVIOUS,
                                       LV_DIR_TOP | LV_DIR_LEFT),
                  "Compare - August", magenta);

  // Home is the entry screen.
  lv_obj_set_tile_id(tv, COL_HOME, ROW_CURRENT, LV_ANIM_OFF);
}

}  // namespace ui
