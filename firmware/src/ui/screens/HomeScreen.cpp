#include "HomeScreen.h"

#include <stdio.h>

#include "../../board/Metrics.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {
namespace {

constexpr int32_t PAD = metrics::PADDING;
constexpr int32_t LEFT_COLUMN = metrics::px(250);
constexpr int32_t RIGHT_X = PAD + LEFT_COLUMN + metrics::px(16);

lv_obj_t* make_label(lv_obj_t* parent, int32_t x, int32_t y, const lv_font_t* font,
                     uint32_t color) {
  lv_obj_t* label = lv_label_create(parent);
  lv_obj_set_pos(label, x, y);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_color(label, lv_color_hex(color), 0);
  lv_label_set_text(label, "");
  return label;
}

void show(lv_obj_t* obj, bool visible) {
  if (obj == nullptr) return;
  if (visible) {
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }
}

}  // namespace

void HomeScreen::build(lv_obj_t* root) {
  title_ = make_label(root, PAD + 54, PAD + 8, fonts::title(), theme::INK);
  badge_ = make_label(root, PAD + 54, PAD + 20, fonts::caption(), theme::MUTED);

  gauge_.attach(root, PAD, PAD + 50, Gauge::WIDTH, Gauge::HEIGHT);

  reading_ = make_label(root, PAD, PAD + 200, fonts::reading(), theme::INK);
  reading_caption_ = make_label(root, PAD, PAD + 252, fonts::caption(), theme::MUTED);

  // Closed-month column: two averages where the dial would have been.
  per_day_ = make_label(root, PAD, PAD + 150, fonts::headline(), theme::INK);
  per_day_caption_ = make_label(root, PAD, PAD + 190, fonts::caption(), theme::MUTED);

  divider_ = lv_obj_create(root);
  lv_obj_remove_style_all(divider_);
  lv_obj_set_pos(divider_, PAD, PAD + 128);
  lv_obj_set_size(divider_, LEFT_COLUMN - 40, 1);
  lv_obj_set_style_bg_color(divider_, lv_color_hex(theme::GRID_LIGHT), 0);
  lv_obj_set_style_bg_opa(divider_, LV_OPA_COVER, 0);

  estimate_ = make_label(root, RIGHT_X, PAD, fonts::caption(), theme::MUTED);
  fill_.attach(root, RIGHT_X, PAD + 40, FillBar::WIDTH, FillBar::HEIGHT);
  progress_caption_ = make_label(root, RIGHT_X, PAD + 252, fonts::caption(), theme::MUTED);
}

void HomeScreen::update(const MeterSnapshot& snapshot) {
  if (root_ == nullptr) return;
  const HomeData& data = select(snapshot);

  lv_label_set_text(title_, data.title);
  lv_label_set_text(badge_, data.badge != nullptr ? data.badge : "");
  show(badge_, data.badge != nullptr);

  char buffer[24];

  const bool live = data.live != nullptr;
  show(gauge_.obj(), live);
  show(divider_, !live);
  show(per_day_, !live);
  show(per_day_caption_, !live);

  if (live) {
    gauge_.set_reading(data.live->kw, data.live->scale_kw);
    lv_label_set_text(reading_, fmt::decimal(buffer, sizeof(buffer), data.live->kw, 2));
    lv_label_set_text(reading_caption_, data.live->caption);
    lv_obj_set_y(reading_, PAD + 200);
    lv_obj_set_y(reading_caption_, PAD + 252);
  } else if (data.averages != nullptr) {
    // The dial is gone, so the averages rise into the space it left.
    lv_label_set_text(reading_, fmt::decimal(buffer, sizeof(buffer), data.averages->kw, 2));
    lv_label_set_text(reading_caption_, "Average draw");
    lv_obj_set_y(reading_, PAD + 60);
    lv_obj_set_y(reading_caption_, PAD + 112);
    lv_label_set_text(per_day_,
                      fmt::kwh_decimal(buffer, sizeof(buffer), data.averages->kwh_per_day));
    lv_label_set_text(per_day_caption_, "Average per day");
  }

  char estimate[40];
  char amount[24];
  snprintf(estimate, sizeof(estimate), "%s estimated\nfor %s",
           fmt::kwh(amount, sizeof(amount), data.progress.estimate_kwh), data.progress.month);
  lv_label_set_text(estimate_, estimate);

  fill_.set_progress(data.progress.used_kwh, data.progress.estimate_kwh,
                     data.progress.closed);

  lv_label_set_text(progress_caption_, data.progress.caption);
  lv_obj_set_style_text_color(
      progress_caption_,
      lv_color_hex(data.progress.closed ? theme::MAGENTA : theme::MUTED), 0);
}

}  // namespace ui
