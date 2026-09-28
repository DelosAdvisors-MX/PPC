#include "Gauge.h"

#include <math.h>

#include "../Draw.h"
#include "../Fonts.h"
#include "../Theme.h"

namespace ui {
namespace {

constexpr int32_t CX = 120;
constexpr int32_t CY = 120;
constexpr int32_t ARC_RADIUS = 96;
constexpr int32_t NEEDLE_RADIUS = 84;
constexpr int32_t ARC_WIDTH = 20;

/**
 * LVGL measures angles clockwise from 3 o'clock, so the upper half runs from
 * 180 (hard left) to 360 (hard right) and the reading is a fraction of that.
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
  const float fraction =
      max_ > 0.0f ? LV_CLAMP(0.0f, value_ / max_, 1.0f) : 0.0f;

  lv_draw_arc_dsc_t arc;
  lv_draw_arc_dsc_init(&arc);
  arc.center.x = local_x(CX);
  arc.center.y = local_y(CY);
  arc.radius = ARC_RADIUS;
  arc.width = ARC_WIDTH;
  arc.rounded = 1;
  arc.opa = LV_OPA_COVER;

  arc.color = lv_color_hex(theme::GAUGE_TRACK);
  arc.start_angle = ANGLE_START;
  arc.end_angle = ANGLE_START + ANGLE_SWEEP;
  lv_draw_arc(layer, &arc);

  if (fraction > 0.001f) {
    arc.color = lv_color_hex(theme::BLUE);
    arc.end_angle = ANGLE_START + static_cast<int32_t>(lroundf(ANGLE_SWEEP * fraction));
    lv_draw_arc(layer, &arc);
  }

  // The needle is drawn in the design's own geometry rather than by angle, so
  // it lands on exactly the same pixel as the web render.
  const float radians = static_cast<float>(M_PI) * fraction;
  const int32_t tip_x = CX - static_cast<int32_t>(lroundf(NEEDLE_RADIUS * cosf(radians)));
  const int32_t tip_y = CY - static_cast<int32_t>(lroundf(NEEDLE_RADIUS * sinf(radians)));
  draw::line(layer, local_x(CX), local_y(CY), local_x(tip_x), local_y(tip_y),
             theme::INK, 7, /*rounded=*/true);

  lv_draw_rect_dsc_t hub;
  lv_draw_rect_dsc_init(&hub);
  hub.bg_color = lv_color_hex(theme::INK);
  hub.bg_opa = LV_OPA_COVER;
  hub.radius = LV_RADIUS_CIRCLE;
  lv_area_t hub_area = {local_x(CX - 10), local_y(CY - 10), local_x(CX + 10), local_y(CY + 10)};
  lv_draw_rect(layer, &hub, &hub_area);

  char scale[12];
  snprintf(scale, sizeof(scale), "%d kW", static_cast<int>(max_));
  draw::text(layer, local_x(20), local_y(132), 40, "0", fonts::caption(), theme::MUTED);
  draw::text(layer, local_x(160), local_y(132), 60, scale, fonts::caption(), theme::MUTED,
             LV_TEXT_ALIGN_RIGHT);
}

}  // namespace ui
