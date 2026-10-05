#include "YearLine.h"

#include <math.h>
#include <stdio.h>

#include "../../board/Metrics.h"
#include "../Draw.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {
namespace {

constexpr int32_t H = 174;
constexpr int32_t BASE = 150;
constexpr int32_t PLOT_LEFT = 40;
constexpr int32_t PLOT_INSET = 12;  // so the last dot is not clipped
constexpr int32_t AXIS_X = 34;
constexpr int MONTHS_IN_YEAR = 12;

/**
 * The y axis is fixed, not fitted: 600 kWh sits at the same height on every
 * year, so 2025 and 2026 can be compared by flicking between them.
 */
constexpr float PX_PER_KWH = (BASE - 33.4f) / 600.0f;
constexpr float CEILING = BASE / PX_PER_KWH;
constexpr int TICKS[] = {0, 200, 400, 600};
constexpr int LABELLED[] = {0, 2, 4, 6, 8, 10};
constexpr const char* MONTHS[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                                  "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

}  // namespace

void YearLine::set_year(const float* monthly, size_t months) {
  monthly_ = monthly;
  months_ = months;
  invalidate();
}

void YearLine::draw(lv_layer_t* layer) {
  if (monthly_ == nullptr || months_ == 0) return;

  const int32_t W = width();
  const int32_t plot_right = W - PLOT_INSET;
  const float step = static_cast<float>(plot_right - PLOT_LEFT) / (MONTHS_IN_YEAR - 1);
  const auto x_of = [&](size_t i) {
    return PLOT_LEFT + static_cast<int32_t>(lroundf(i * step));
  };
  const auto y_of = [&](float v) {
    return static_cast<int32_t>(lroundf(BASE - fminf(v, CEILING) * PX_PER_KWH));
  };

  float peak = monthly_[0];
  size_t peak_index = 0;
  for (size_t i = 0; i < months_; i++) {
    if (monthly_[i] > peak) {
      peak = monthly_[i];
      peak_index = i;
    }
  }

  // The fill goes down first so the gridlines can be read across it. LVGL has
  // no polygon fill, so the area is a column of one-pixel rects under the
  // line — which is what a polygon fill would rasterise to anyway.
  for (int32_t x = x_of(0); x <= x_of(months_ - 1); x++) {
    const float t = (x - PLOT_LEFT) / step;
    const size_t i = static_cast<size_t>(t);
    const size_t next = i + 1 < months_ ? i + 1 : i;
    const float value = monthly_[i] + (monthly_[next] - monthly_[i]) * (t - i);
    const int32_t y = y_of(value);
    draw::rect(layer, local_x(x), local_y(y), 1, BASE - y, theme::BLUE_WASH);
  }

  char label[12];
  for (const int tick : TICKS) {
    const int32_t y = y_of(tick);
    draw::line(layer, local_x(AXIS_X), local_y(y), local_x(W), local_y(y),
               tick == 0 ? theme::AXIS : theme::GRID_LIGHT);
    snprintf(label, sizeof(label), "%d", tick);
    draw::text(layer, local_x(0), local_y(y - fonts::caption()->line_height / 2), AXIS_X - 6,
               label, fonts::caption(), theme::MUTED, LV_TEXT_ALIGN_RIGHT);
  }

  for (size_t i = 0; i + 1 < months_; i++) {
    draw::line(layer, local_x(x_of(i)), local_y(y_of(monthly_[i])), local_x(x_of(i + 1)),
               local_y(y_of(monthly_[i + 1])), theme::BLUE, 3, true);
  }

  for (const int i : LABELLED) {
    if (static_cast<size_t>(i) >= months_) continue;
    draw::circle(layer, local_x(x_of(i)), local_y(y_of(monthly_[i])), metrics::px(4),
                 theme::BLUE);
  }

  draw::circle(layer, local_x(x_of(peak_index)), local_y(y_of(peak)), metrics::px(5),
               theme::MAGENTA, theme::SURFACE, 2);

  // The peak figure sits above its dot unless the year peaks near the top of
  // the scale, where there is no room — then it moves alongside.
  fmt::thousands(label, sizeof(label), peak);
  const int32_t peak_y = y_of(peak);
  const bool above = peak_y - 22 >= 0;
  draw::halo_text(layer,
                  local_x(above ? x_of(peak_index) - 30
                                : x_of(peak_index) + (peak_index > months_ / 2 ? -70 : 10)),
                  local_y(above ? peak_y - 22 : peak_y - 10), 60, label, fonts::caption(),
                  theme::MAGENTA, theme::SURFACE,
                  above ? LV_TEXT_ALIGN_CENTER
                        : (peak_index > months_ / 2 ? LV_TEXT_ALIGN_RIGHT : LV_TEXT_ALIGN_LEFT));

  for (const int i : LABELLED) {
    // Months still to come keep their label but fade.
    draw::text(layer, local_x(x_of(i) - 25), local_y(H - 22), 50, MONTHS[i], fonts::caption(),
               static_cast<size_t>(i) < months_ ? theme::MUTED : theme::AXIS,
               LV_TEXT_ALIGN_CENTER);
  }
}

}  // namespace ui
