#include "HighScreen.h"

#include <stdio.h>

#include "../../board/Metrics.h"
#include "../Card.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {

void HighScreen::build(lv_obj_t* root) {
  header_.build(root, metrics::PADDING);

  lv_obj_t* card = make_card(root, metrics::PADDING + metrics::px(64));

  unit_ = make_eyebrow(card, 0, 0);
  total_ = lv_label_create(card);
  lv_obj_align(total_, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_text_font(total_, fonts::caption(), 0);
  lv_obj_set_style_text_color(total_, lv_color_hex(theme::INK), 0);

  line_.attach(card, 0, metrics::px(24), metrics::chart::CARD, metrics::px(174));
}

void HighScreen::update(const MeterSnapshot& snapshot) {
  if (root_ == nullptr) return;
  const YearInsight& data = select(snapshot);

  float total = 0.0f;
  for (size_t i = 0; i < data.months; i++) total += data.monthly[i];

  char stat[16];
  fmt::kwh(stat, sizeof(stat), data.months > 0 ? total / data.months : 0.0f);
  header_.set(data.title, stat, "Avg. monthly");

  lv_label_set_text(unit_, "KWH A MONTH");

  // A year in progress has no year total yet, only a running one.
  char figure[16];
  char caption[40];
  snprintf(caption, sizeof(caption), "%s %s kWh", data.months >= 12 ? "Total year" : "Total so far",
           fmt::thousands(figure, sizeof(figure), total));
  lv_label_set_text(total_, caption);

  line_.set_year(data.monthly, data.months);
}

}  // namespace ui
