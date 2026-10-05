#include "Gauge.h"

#include <math.h>
#include <stdio.h>

#include "../../board/Metrics.h"
#include "../Draw.h"
#include "../Fonts.h"
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
 * LVGL measures angles clockwise from 3 o'clock, so the upper half runs 180 to
 * 360 and the reading is a fraction of that.
 */
constexpr int32_t ANGLE_START = 180;
constexpr int32_t ANGLE_SWEEP = 180;

}  // namespace

void Gauge::set_reading(float value, float max) {
  value_ = value;
  max_ = max;
  invalidate();
}

void Gauge::draw(lv_layer_t* layer) {
  const int32_t W = width();
  const int32_t cx = W / 2;
  const int32_t cy = metrics::px(126);
  const int32_t radius = metrics::px(102);
  const int32_t arc_width = metrics::px(30);
  const int32_t needle_r = metrics::px(86);

  const float fraction = max_ > 0.0f ? fminf(fmaxf(value_ / max_, 0.0f), 1.0f) : 0.0f;

  lv_draw_arc_dsc_t arc;
  lv_draw_arc_dsc_init(&arc);
  arc.center.x = local_x(cx);
  arc.center.y = local_y(cy);
  arc.radius = radius;
  arc.width = arc_width;
  arc.rounded = 1;
  arc.opa = LV_OPA_COVER;

  arc.color = lv_color_hex(theme::GAUGE_TRACK);
  arc.start_angle = ANGLE_START;
  arc.end_angle = ANGLE_START + ANGLE_SWEEP;
  lv_draw_arc(layer, &arc);

  if (fraction > 0.001f) {
    arc.color = lv_color_hex(load_color(fraction));
    arc.end_angle = ANGLE_START + static_cast<int32_t>(lroundf(ANGLE_SWEEP * fraction));
    lv_draw_arc(layer, &arc);
  }

  // The needle is placed by the design's own geometry rather than by angle, so
  // it lands where the web render puts it.
  const float radians = static_cast<float>(M_PI) * fraction;
  const int32_t tip_x = cx - static_cast<int32_t>(lroundf(needle_r * cosf(radians)));
  const int32_t tip_y = cy - static_cast<int32_t>(lroundf(needle_r * sinf(radians)));
  draw::line(layer, local_x(cx), local_y(cy), local_x(tip_x), local_y(tip_y), theme::INK,
             metrics::px(8), true);
  draw::circle(layer, local_x(cx), local_y(cy), metrics::px(12), theme::INK);

  char scale[12];
  snprintf(scale, sizeof(scale), "%d kW", static_cast<int>(max_));
  const int32_t label_y = metrics::px(140);
  draw::text(layer, local_x(metrics::px(10)), local_y(label_y), metrics::px(40), "0",
             fonts::caption(), theme::MUTED);
  draw::text(layer, local_x(W - metrics::px(70)), local_y(label_y), metrics::px(60), scale,
             fonts::caption(), theme::MUTED, LV_TEXT_ALIGN_RIGHT);
}

}  // namespace ui
