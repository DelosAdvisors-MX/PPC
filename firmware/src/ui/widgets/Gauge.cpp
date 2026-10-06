#include "Gauge.h"

#include <math.h>
#include <stdio.h>

#include "../../board/Metrics.h"
#include "../Draw.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {
namespace {

/** Where the dial changes colour, as a fraction of full scale. */
constexpr float BAND_LOW = 0.40f;
constexpr float BAND_MID = 0.70f;

uint32_t load_color(float fraction) {
  if (fraction < BAND_LOW) return theme::LOAD_LOW;
  if (fraction < BAND_MID) return theme::LOAD_MID;
  return theme::LOAD_HIGH;
}

/**
 * A speed-test dial: it opens at the bottom and sweeps 270 degrees, which
 * leaves room for graduations the whole way round.
 *
 * LVGL measures angles clockwise from 3 o'clock with y down, so 135 is the
 * lower-left foot and 405 — 45 past the wrap — is the lower-right one.
 */
constexpr int32_t START = 135;
constexpr int32_t SWEEP = 270;

float radians_at(int32_t degrees) {
  return degrees * static_cast<float>(M_PI) / 180.0f;
}

}  // namespace

void Gauge::set_reading(float value, float max) {
  value_ = value;
  max_ = max;
  invalidate();
}

void Gauge::draw(lv_layer_t* layer) {
  const int32_t W = width();
  const int32_t cx = W / 2;
  const int32_t cy = metrics::px(112);
  const int32_t radius = metrics::px(86);
  const int32_t arc_width = metrics::px(26);
  const int32_t needle_r = metrics::px(66);
  const int32_t tick_inner = radius + arc_width / 2 + metrics::px(3);
  const int32_t tick_outer = tick_inner + metrics::px(7);
  const int32_t label_r = tick_outer + metrics::px(13);

  const float fraction = max_ > 0.0f ? fminf(fmaxf(value_ / max_, 0.0f), 1.0f) : 0.0f;
  const auto angle_at = [&](float f) {
    return START + static_cast<int32_t>(lroundf(SWEEP * f));
  };
  const auto point = [&](int32_t degrees, int32_t r, int32_t& x, int32_t& y) {
    x = cx + static_cast<int32_t>(lroundf(r * cosf(radians_at(degrees))));
    y = cy + static_cast<int32_t>(lroundf(r * sinf(radians_at(degrees))));
  };

  lv_draw_arc_dsc_t arc;
  lv_draw_arc_dsc_init(&arc);
  arc.center.x = local_x(cx);
  arc.center.y = local_y(cy);
  arc.radius = radius;
  arc.width = arc_width;
  arc.rounded = 1;
  arc.opa = LV_OPA_COVER;

  // Drawn in two pieces where the sweep crosses 360, rather than trusting the
  // driver to wrap an end angle past a full turn.
  const auto sweep_arc = [&](int32_t from, int32_t to, uint32_t color) {
    if (to <= from) return;
    arc.color = lv_color_hex(color);
    if (to <= 360) {
      arc.start_angle = from;
      arc.end_angle = to;
      lv_draw_arc(layer, &arc);
      return;
    }
    arc.start_angle = from;
    arc.end_angle = 360;
    lv_draw_arc(layer, &arc);
    arc.start_angle = 0;
    arc.end_angle = to - 360;
    lv_draw_arc(layer, &arc);
  };

  sweep_arc(START, START + SWEEP, theme::GAUGE_TRACK);
  if (fraction > 0.002f) sweep_arc(START, angle_at(fraction), load_color(fraction));

  // A graduation every half kilowatt, numbered on the whole ones.
  const int steps = static_cast<int>(lroundf(max_ * 2));
  char label[8];
  for (int i = 0; i <= steps; i++) {
    const float kw = i / 2.0f;
    const bool major = (i % 2) == 0;
    const int32_t degrees = angle_at(max_ > 0.0f ? kw / max_ : 0.0f);

    int32_t x1, y1, x2, y2;
    point(degrees, major ? tick_inner : tick_inner + metrics::px(3), x1, y1);
    point(degrees, tick_outer, x2, y2);
    draw::line(layer, local_x(x1), local_y(y1), local_x(x2), local_y(y2),
               major ? theme::MUTED : theme::AXIS, major ? 2 : 1, true);

    if (!major) continue;
    int32_t lx, ly;
    point(degrees, label_r, lx, ly);
    snprintf(label, sizeof(label), "%d", static_cast<int>(kw));
    draw::text(layer, local_x(lx - metrics::px(10)),
               local_y(ly - fonts::caption()->line_height / 2), metrics::px(20), label,
               fonts::caption(), theme::MUTED, LV_TEXT_ALIGN_CENTER);
  }

  int32_t nx, ny;
  point(angle_at(fraction), needle_r, nx, ny);
  draw::line(layer, local_x(cx), local_y(cy), local_x(nx), local_y(ny), theme::INK,
             metrics::px(7), true);
  draw::circle(layer, local_x(cx), local_y(cy), metrics::px(10), theme::INK);

  // The reading sits below the pivot, in the mouth of the dial: the sweep runs
  // 135 to 405, so the needle never points straight down and that is the one
  // place inside the dial it cannot reach.
  char reading[12];
  fmt::decimal(reading, sizeof(reading), value_, 2);
  draw::text(layer, local_x(cx - W / 2), local_y(cy + metrics::px(18)), W, reading,
             fonts::reading(), theme::INK, LV_TEXT_ALIGN_CENTER);
  draw::text(layer, local_x(cx - W / 2), local_y(cy + metrics::px(56)), W, "kW",
             fonts::caption(), theme::MUTED, LV_TEXT_ALIGN_CENTER);
}

}  // namespace ui
