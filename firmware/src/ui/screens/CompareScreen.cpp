#include "CompareScreen.h"

#include <stdio.h>

#include "../../board/Metrics.h"
#include "../Card.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {

void CompareScreen::build(lv_obj_t* root) {
  const int32_t column = metrics::COMPARE_COLUMN;

  eyebrow_ = make_eyebrow(root, metrics::PADDING, metrics::PADDING);
  lv_obj_set_style_text_color(eyebrow_, lv_color_hex(theme::MUTED), 0);

  percent_ = lv_label_create(root);
  lv_obj_set_pos(percent_, metrics::PADDING, metrics::PADDING + metrics::px(14));
  lv_obj_set_style_text_font(percent_, fonts::reading(), 0);
  lv_obj_set_style_text_color(percent_, lv_color_hex(theme::MAGENTA), 0);

  direction_ = lv_label_create(root);
  lv_obj_set_pos(direction_, metrics::PADDING, metrics::PADDING + metrics::px(76));
  lv_obj_set_style_text_font(direction_, fonts::headline(), 0);
  lv_obj_set_style_text_color(direction_, lv_color_hex(theme::MAGENTA), 0);

  subtitle_ = lv_label_create(root);
  lv_obj_set_pos(subtitle_, metrics::PADDING, metrics::PADDING + metrics::px(126));
  lv_obj_set_width(subtitle_, column);
  lv_label_set_long_mode(subtitle_, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_font(subtitle_, fonts::micro(), 0);
  lv_obj_set_style_text_color(subtitle_, lv_color_hex(theme::INK), 0);

  // Two chips a row, which is what fits the column at this weight.
  for (size_t i = 0; i < MAX_CHIPS; i++) {
    lv_obj_t* chip = lv_obj_create(root);
    lv_obj_remove_style_all(chip);
    lv_obj_set_pos(chip, metrics::PADDING + static_cast<int32_t>(i % 2) * (column / 2),
                   metrics::PADDING + metrics::px(166) +
                       static_cast<int32_t>(i / 2) * metrics::px(22));
    lv_obj_set_size(chip, column / 2 - metrics::px(4), metrics::px(18));
    lv_obj_set_style_radius(chip, metrics::px(9), 0);
    lv_obj_set_style_bg_color(chip, lv_color_hex(theme::WASH), 0);
    lv_obj_set_style_bg_opa(chip, LV_OPA_COVER, 0);
    lv_obj_remove_flag(chip, LV_OBJ_FLAG_SCROLLABLE);
    chips_[i] = chip;

    lv_obj_t* label = lv_label_create(chip);
    lv_obj_center(label);
    lv_obj_set_style_text_font(label, fonts::micro(), 0);
    lv_obj_set_style_text_color(label, lv_color_hex(theme::INK), 0);
    chip_labels_[i] = label;
  }

  group_ = lv_label_create(root);
  lv_obj_set_pos(group_, metrics::PADDING, metrics::HEIGHT - metrics::PADDING - metrics::px(18));
  lv_obj_set_style_text_font(group_, fonts::micro(), 0);
  lv_obj_set_style_text_color(group_, lv_color_hex(theme::CHIP_INK), 0);

  lv_obj_t* card = make_card(root, metrics::PADDING,
                             metrics::CONTENT_WIDTH - column - metrics::COMPARE_GAP,
                             metrics::PADDING + column + metrics::COMPARE_GAP);
  unit_ = make_eyebrow(card, 0, 0);
  bars_.attach(card, 0, metrics::px(22), metrics::chart::COMPARE, metrics::px(238));
}

void CompareScreen::update(const MeterSnapshot& snapshot) {
  if (root_ == nullptr) return;
  const CompareData& data = select(snapshot);

  const int delta = fmt::percent_higher(data.my_kwh, data.their_kwh);
  const bool higher = delta >= 0;
  const uint32_t accent = higher ? theme::MAGENTA : theme::BLUE;

  lv_label_set_text(eyebrow_, data.eyebrow != nullptr ? data.eyebrow : "");

  char label[40];
  snprintf(label, sizeof(label), "%d%%", delta < 0 ? -delta : delta);
  lv_label_set_text(percent_, label);
  lv_obj_set_style_text_color(percent_, lv_color_hex(accent), 0);

  lv_label_set_text(direction_, higher ? "higher" : "lower");
  lv_obj_set_style_text_color(direction_, lv_color_hex(accent), 0);

  lv_label_set_text(subtitle_, data.subtitle);

  for (size_t i = 0; i < MAX_CHIPS; i++) {
    const bool used = i < data.chip_count;
    lv_obj_set_style_opa(chips_[i], used ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
    if (used) lv_label_set_text(chip_labels_[i], data.chips[i]);
  }

  char figure[16];
  snprintf(label, sizeof(label), "%s similar homes",
           fmt::thousands(figure, sizeof(figure), static_cast<float>(data.group_size)));
  lv_label_set_text(group_, label);

  lv_label_set_text(unit_, data.unit_label);
  bars_.set_pair(data.my_kwh, data.their_kwh);
}

}  // namespace ui
