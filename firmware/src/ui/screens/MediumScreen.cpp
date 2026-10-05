#include "MediumScreen.h"

#include "../../board/Metrics.h"
#include "../Card.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {

void MediumScreen::build(lv_obj_t* root) {
  header_.build(root, metrics::PADDING);

  const int32_t card_y = metrics::PADDING + metrics::px(64);
  lv_obj_t* card = make_card(root, card_y);

  unit_ = make_eyebrow(card, 0, 0);
  legend_ = lv_label_create(card);
  lv_obj_align(legend_, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_style_text_font(legend_, fonts::micro(), 0);
  lv_obj_set_style_text_color(legend_, lv_color_hex(theme::MUTED), 0);
  lv_label_set_text(legend_, "- - average      peak day");

  bars_.attach(card, 0, metrics::px(24), metrics::chart::CARD, metrics::px(174));
}

void MediumScreen::update(const MeterSnapshot& snapshot) {
  if (root_ == nullptr) return;
  const DailyInsight& data = select(snapshot);

  float total = 0.0f;
  for (size_t i = 0; i < data.days; i++) total += data.daily[i];

  char stat[16];
  fmt::kwh_decimal(stat, sizeof(stat), data.days > 0 ? total / data.days : 0.0f);
  header_.set(data.title, stat, "Avg. daily");

  lv_label_set_text(unit_, "KWH A DAY");
  bars_.set_month(data.daily, data.days, data.days_in_month);
}

}  // namespace ui
