#include "TelegramScreen.h"

#include <string.h>

#include "../../board/Metrics.h"
#include "../Fonts.h"
#include "../Theme.h"

namespace ui {

void TelegramScreen::build(lv_obj_t* root) {
  title_ = lv_label_create(root);
  lv_obj_set_pos(title_, metrics::PADDING, metrics::PADDING);
  lv_obj_set_style_text_font(title_, fonts::title(), 0);
  lv_obj_set_style_text_color(title_, lv_color_hex(theme::INK), 0);
  lv_label_set_text(title_, "Your smart meter P1 telegram");

  const int32_t panel_y = metrics::PADDING + metrics::px(36);
  lv_obj_t* panel = lv_obj_create(root);
  lv_obj_remove_style_all(panel);
  lv_obj_set_pos(panel, metrics::PADDING, panel_y);
  lv_obj_set_size(panel, metrics::CONTENT_WIDTH, metrics::HEIGHT - panel_y - metrics::PADDING);
  lv_obj_set_style_radius(panel, metrics::px(10), 0);
  lv_obj_set_style_bg_color(panel, lv_color_hex(theme::TERM_BG), 0);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, 0);
  lv_obj_set_style_pad_hor(panel, metrics::px(12), 0);
  lv_obj_set_style_pad_ver(panel, metrics::px(10), 0);
  lv_obj_remove_flag(panel, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t* live = lv_obj_create(panel);
  lv_obj_remove_style_all(live);
  lv_obj_set_pos(live, 0, metrics::px(4));
  lv_obj_set_size(live, metrics::px(7), metrics::px(7));
  lv_obj_set_style_radius(live, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_bg_color(live, lv_color_hex(theme::TERM_LIVE), 0);
  lv_obj_set_style_bg_opa(live, LV_OPA_COVER, 0);

  lv_obj_t* header = lv_label_create(panel);
  lv_obj_set_pos(header, metrics::px(14), 0);
  lv_obj_set_style_text_font(header, fonts::micro(), 0);
  lv_obj_set_style_text_color(header, lv_color_hex(theme::TERM_LABEL), 0);
  lv_label_set_text(header, "DSMR 5.0 Telegram");

  lv_obj_t* raw = lv_label_create(panel);
  lv_obj_align(raw, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_text_font(raw, fonts::micro(), 0);
  lv_obj_set_style_text_color(raw, lv_color_hex(theme::TERM_DIM), 0);
  lv_label_set_text(raw, "raw output");

  const int32_t pitch = metrics::px(19);
  const int32_t first = metrics::px(22);
  for (size_t i = 0; i < MAX_LINES; i++) {
    const int32_t y = first + static_cast<int32_t>(i) * pitch;

    codes_[i] = lv_label_create(panel);
    lv_obj_set_pos(codes_[i], 0, y);
    lv_obj_set_style_text_font(codes_[i], fonts::micro(), 0);
    lv_obj_set_style_text_color(codes_[i], lv_color_hex(theme::TERM_TEXT), 0);

    glosses_[i] = lv_label_create(panel);
    lv_obj_set_width(glosses_[i], metrics::CONTENT_WIDTH - metrics::px(24));
    lv_obj_set_pos(glosses_[i], 0, y);
    lv_obj_set_style_text_align(glosses_[i], LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_font(glosses_[i], fonts::micro(), 0);
    lv_obj_set_style_text_color(glosses_[i], lv_color_hex(theme::TERM_DIM), 0);
  }
}

void TelegramScreen::update(const MeterSnapshot& snapshot) {
  if (root_ == nullptr) return;
  const TelegramData& data = snapshot.telegram;

  for (size_t i = 0; i < MAX_LINES; i++) {
    const bool used = i < data.count;
    lv_obj_set_style_opa(codes_[i], used ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    lv_obj_set_style_opa(glosses_[i], used ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    if (!used) continue;

    const TelegramLine& line = data.lines[i];
    // The highlight is a prefix, not a whole line: the value is rewritten
    // every tick once this is fed from a live meter.
    const bool lit = strncmp(line.code, data.highlight, strlen(data.highlight)) == 0;

    lv_label_set_text(codes_[i], line.code);
    lv_obj_set_style_text_color(codes_[i],
                                lv_color_hex(lit ? theme::TERM_HIGHLIGHT : theme::TERM_TEXT), 0);
    lv_label_set_text(glosses_[i], line.gloss);
    lv_obj_set_style_text_color(glosses_[i],
                                lv_color_hex(lit ? theme::TERM_HIGHLIGHT : theme::TERM_DIM), 0);
  }
}

}  // namespace ui
