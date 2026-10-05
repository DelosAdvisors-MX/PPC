#include "FillBar.h"

#include <math.h>

#include "../Draw.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {
namespace {

constexpr int32_t PLOT_LEFT = 42;
constexpr int32_t TOP = 12;   // the 100% line
constexpr int32_t BASE = 196; // the 0% line
constexpr int32_t SPAN = BASE - TOP;
/** The bar takes this share of the plot, centred. */
constexpr float BAR_SHARE = 0.571f;
constexpr int32_t RADIUS = 8;

}  // namespace

void FillBar::set_progress(float used_kwh, float estimate_kwh, bool closed) {
  used_ = used_kwh;
  estimate_ = estimate_kwh;
  closed_ = closed;
  invalidate();
}

void FillBar::draw(lv_layer_t* layer) {
  const int32_t W = width();
  const int32_t plot_width = W - PLOT_LEFT;
  const int32_t bar_w = static_cast<int32_t>(plot_width * BAR_SHARE);
  const int32_t bar_x = PLOT_LEFT + (plot_width - bar_w) / 2;
  const float ratio = estimate_ > 0.0f ? used_ / estimate_ : 0.0f;
  const bool over = ratio > 1.0f;
  const int32_t top =
      LV_MAX(TOP, BASE - static_cast<int32_t>(lroundf(LV_MIN(ratio, 1.0f) * SPAN)));
  const uint32_t accent = closed_ ? theme::MAGENTA : theme::BLUE;

  if (!over) {
    draw::line(layer, local_x(PLOT_LEFT), local_y(TOP), local_x(W), local_y(TOP),
               theme::GRID_LIGHT);
  }
  draw::line(layer, local_x(PLOT_LEFT), local_y(104), local_x(W), local_y(104),
             theme::GRID_LIGHT);
  draw::line(layer, local_x(PLOT_LEFT), local_y(BASE), local_x(W), local_y(BASE),
             theme::AXIS);

  struct Tick {
    const char* label;
    int32_t y;
  };
  constexpr Tick TICKS[] = {{"100%", 4}, {"50%", 96}, {"0%", 188}};
  for (const Tick& tick : TICKS) {
    draw::text(layer, local_x(0), local_y(tick.y), 36, tick.label, fonts::caption(),
               theme::MUTED, LV_TEXT_ALIGN_RIGHT);
  }

  draw::top_rounded_bar(layer, local_x(bar_x), local_y(top), bar_w, BASE - top, RADIUS,
                        accent);

  if (over) {
    // Drawn over the bar, so "you went past the estimate" is unmistakable.
    draw::line(layer, local_x(PLOT_LEFT), local_y(TOP), local_x(W), local_y(TOP),
               theme::INK, 2, false, 4, 3);
  }

  // Above the bar normally; once the bar reaches the ceiling there is no room
  // left, so the figure moves inside it and flips to white.
  const bool inside = top < 24;
  char label[16];
  fmt::kwh(label, sizeof(label), used_);
  draw::text(layer, local_x(bar_x), local_y(inside ? top + 10 : top - 24), bar_w, label,
             fonts::body(), inside ? theme::SURFACE : theme::INK, LV_TEXT_ALIGN_CENTER);
}

}  // namespace ui
