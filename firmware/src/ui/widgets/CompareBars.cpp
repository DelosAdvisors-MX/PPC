#include "CompareBars.h"

#include <math.h>

#include "../../board/Metrics.h"
#include "../Draw.h"
#include "../Fonts.h"
#include "../Format.h"
#include "../Theme.h"

namespace ui {
namespace {

constexpr int32_t BASE = 190;
constexpr int32_t TALLEST = 174;
constexpr int32_t RADIUS = 7;
/** Bar width and the gap between, as shares of the chart. */
constexpr float BAR_SHARE = 0.38f;
constexpr float GAP_SHARE = 0.11f;

}  // namespace

void CompareBars::set_pair(float mine, float theirs) {
  mine_ = mine;
  theirs_ = theirs;
  invalidate();
}

void CompareBars::draw(lv_layer_t* layer) {
  const int32_t W = width();
  const int32_t bar_w = static_cast<int32_t>(lroundf(W * BAR_SHARE));
  const int32_t pair_w = bar_w * 2 + static_cast<int32_t>(lroundf(W * GAP_SHARE));
  const int32_t mine_x = (W - pair_w) / 2;
  const int32_t theirs_x = mine_x + pair_w - bar_w;

  const float tallest = fmaxf(fmaxf(mine_, theirs_), 1.0f);
  const auto y_of = [&](float v) {
    return static_cast<int32_t>(lroundf(BASE - (v / tallest) * TALLEST));
  };

  draw::line(layer, local_x(0), local_y(BASE), local_x(W), local_y(BASE), theme::AXIS);

  struct Bar {
    int32_t x;
    float value;
    uint32_t color;
  };
  const Bar bars[] = {{mine_x, mine_, theme::MAGENTA}, {theirs_x, theirs_, theme::NEUTRAL_BAR}};

  char label[12];
  for (const Bar& bar : bars) {
    const int32_t y = y_of(bar.value);
    draw::top_rounded_bar(layer, local_x(bar.x), local_y(y), bar_w, BASE - y, RADIUS, bar.color);
    fmt::thousands(label, sizeof(label), bar.value);
    draw::text(layer, local_x(bar.x), local_y(y - metrics::px(22)), bar_w, label,
               fonts::body(), theme::INK, LV_TEXT_ALIGN_CENTER);
  }

  draw::text(layer, local_x(mine_x - 20), local_y(BASE + metrics::px(8)), bar_w + 40,
             "Your home", fonts::caption(), theme::INK, LV_TEXT_ALIGN_CENTER);
  draw::text(layer, local_x(theirs_x - 20), local_y(BASE + metrics::px(8)), bar_w + 40,
             "Similar homes", fonts::caption(), theme::MUTED, LV_TEXT_ALIGN_CENTER);
}

}  // namespace ui
