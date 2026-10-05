#include "DailyBars.h"

#include <math.h>
#include <stdio.h>

#include "../../board/Metrics.h"
#include "../Draw.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Scale.h"
#include "../Theme.h"

namespace ui {
namespace {

constexpr int32_t H = 174;
constexpr int32_t BASE = 150;  // the zero line; bars grow up from here
constexpr int32_t TOP = 14;    // where the top tick sits
constexpr int32_t AXIS_X = 34; // left of this is the scale
constexpr int32_t RADIUS = 3;
constexpr int DAY_TICKS[] = {1, 8, 15, 22, 29};

}  // namespace

void DailyBars::set_month(const float* daily, size_t days, size_t days_in_month) {
  daily_ = daily;
  days_ = days;
  days_in_month_ = days_in_month;
  invalidate();
}

void DailyBars::draw(lv_layer_t* layer) {
  if (daily_ == nullptr || days_ == 0) return;

  float peak = daily_[0];
  size_t peak_index = 0;
  float total = 0.0f;
  for (size_t i = 0; i < days_; i++) {
    total += daily_[i];
    if (daily_[i] > peak) {
      peak = daily_[i];
      peak_index = i;
    }
  }
  const float average = total / days_;

  const int32_t W = width();
  const float scale_x = static_cast<float>(W - AXIS_X);
  const Ticks ticks = zero_based_ticks(peak);
  const auto y_of = [&](float value) {
    return static_cast<int32_t>(lroundf(BASE - (value / ticks.max) * (BASE - TOP)));
  };

  const size_t slots = days_in_month_ > days_ ? days_in_month_ : days_;
  const float band = scale_x / slots;
  const int32_t bar_w = static_cast<int32_t>(lroundf(fminf(metrics::px(11), band * 0.72f)));
  const auto bar_x = [&](size_t i) {
    return AXIS_X + static_cast<int32_t>(lroundf(i * band + (band - bar_w) / 2.0f));
  };
  const auto centre = [&](size_t i) {
    return AXIS_X + static_cast<int32_t>(lroundf(i * band + band / 2.0f));
  };

  char label[12];

  for (int t = 0; t < ticks.count; t++) {
    const float value = t * ticks.step;
    const int32_t y = y_of(value);
    draw::line(layer, local_x(AXIS_X), local_y(y), local_x(W), local_y(y),
               t == 0 ? theme::AXIS : theme::GRID_FAINT);
    snprintf(label, sizeof(label), "%d", static_cast<int>(lroundf(value)));
    draw::text(layer, local_x(0), local_y(y - fonts::caption()->line_height / 2), AXIS_X - 6,
               label, fonts::caption(), theme::MUTED, LV_TEXT_ALIGN_RIGHT);
  }

  for (size_t i = 0; i < days_; i++) {
    const int32_t y = y_of(daily_[i]);
    draw::top_rounded_bar(layer, local_x(bar_x(i)), local_y(y), bar_w, BASE - y, RADIUS,
                          i == peak_index ? theme::MAGENTA : theme::BLUE);
  }

  const int32_t average_y = y_of(average);
  draw::line(layer, local_x(AXIS_X), local_y(average_y), local_x(W), local_y(average_y),
             theme::MUTED, 2, false, 5, 4);

  fmt::decimal(label, sizeof(label), peak);
  draw::halo_text(layer, local_x(centre(peak_index) - 30), local_y(y_of(peak) - 20), 60, label,
                  fonts::caption(), theme::MAGENTA, theme::SURFACE, LV_TEXT_ALIGN_CENTER);

  for (const int day : DAY_TICKS) {
    if (static_cast<size_t>(day) > days_in_month_) continue;
    snprintf(label, sizeof(label), "%d", day);
    // Days still to come keep their label but fade, so the gap reads as
    // "not yet" rather than as a chart that stops early.
    draw::text(layer, local_x(centre(day - 1) - 20), local_y(H - 22), 40, label,
               fonts::caption(),
               static_cast<size_t>(day) <= days_ ? theme::MUTED : theme::AXIS,
               LV_TEXT_ALIGN_CENTER);
  }
}

}  // namespace ui
