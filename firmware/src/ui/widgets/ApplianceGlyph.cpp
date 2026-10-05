#include "ApplianceGlyph.h"

#include "../Draw.h"
#include "../Theme.h"

namespace ui {

void ApplianceGlyph::set_icon(ApplianceIcon icon) {
  icon_ = icon;
  invalidate();
}

void ApplianceGlyph::draw(lv_layer_t* layer) {
  const int32_t s = width();              // the glyph is square
  const auto u = [&](float f) { return static_cast<int32_t>(f * s); };
  const int32_t stroke = s >= 30 ? 3 : 2;
  const uint32_t ink = theme::INK;

  const auto box = [&](float x, float y, float w, float h, float r) {
    // An outline, drawn as a filled rounded rect with a lighter one inside.
    draw::rect(layer, local_x(u(x)), local_y(u(y)), u(w), u(h), ink, u(r));
    draw::rect(layer, local_x(u(x) + stroke), local_y(u(y) + stroke), u(w) - 2 * stroke,
               u(h) - 2 * stroke, theme::SURFACE, u(r));
  };
  const auto ring = [&](float cx, float cy, float r) {
    draw::circle(layer, local_x(u(cx)), local_y(u(cy)), u(r), theme::SURFACE, ink, stroke);
  };
  const auto dot = [&](float cx, float cy, float r) {
    draw::circle(layer, local_x(u(cx)), local_y(u(cy)), u(r), ink);
  };

  switch (icon_) {
    case ApplianceIcon::HotWater:
      // A droplet: a round belly with two edges meeting at the point.
      ring(0.5f, 0.62f, 0.26f);
      draw::line(layer, local_x(u(0.5f)), local_y(u(0.12f)), local_x(u(0.27f)),
                 local_y(u(0.60f)), ink, stroke, true);
      draw::line(layer, local_x(u(0.5f)), local_y(u(0.12f)), local_x(u(0.73f)),
                 local_y(u(0.60f)), ink, stroke, true);
      // Close the top of the belly so the two shapes read as one outline.
      draw::rect(layer, local_x(u(0.30f)), local_y(u(0.30f)), u(0.40f), u(0.22f),
                 theme::SURFACE);
      break;

    case ApplianceIcon::TumbleDryer:
      box(0.10f, 0.10f, 0.80f, 0.80f, 0.14f);
      ring(0.50f, 0.56f, 0.24f);
      draw::line(layer, local_x(u(0.26f)), local_y(u(0.25f)), local_x(u(0.38f)),
                 local_y(u(0.25f)), ink, stroke, true);
      break;

    case ApplianceIcon::Oven:
      box(0.10f, 0.14f, 0.80f, 0.72f, 0.10f);
      draw::line(layer, local_x(u(0.10f)), local_y(u(0.36f)), local_x(u(0.90f)),
                 local_y(u(0.36f)), ink, stroke);
      dot(0.26f, 0.25f, 0.045f);
      box(0.26f, 0.48f, 0.48f, 0.26f, 0.06f);
      break;

    case ApplianceIcon::Hob:
      box(0.10f, 0.10f, 0.80f, 0.80f, 0.14f);
      dot(0.35f, 0.35f, 0.075f);
      dot(0.65f, 0.35f, 0.075f);
      dot(0.35f, 0.65f, 0.075f);
      dot(0.65f, 0.65f, 0.075f);
      break;

    case ApplianceIcon::Lighting:
      ring(0.50f, 0.42f, 0.26f);
      draw::line(layer, local_x(u(0.36f)), local_y(u(0.76f)), local_x(u(0.64f)),
                 local_y(u(0.76f)), ink, stroke, true);
      draw::line(layer, local_x(u(0.42f)), local_y(u(0.88f)), local_x(u(0.58f)),
                 local_y(u(0.88f)), ink, stroke, true);
      break;
  }
}

}  // namespace ui
