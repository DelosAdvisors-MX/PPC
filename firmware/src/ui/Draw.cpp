#include "Draw.h"

namespace draw {

void top_rounded_bar(lv_layer_t* layer, int32_t x, int32_t y, int32_t w, int32_t h,
                     int32_t radius, uint32_t color) {
  if (h <= 0 || w <= 0) return;
  const int32_t r = LV_MIN(radius, LV_MIN(w / 2, h));

  lv_draw_rect_dsc_t dsc;
  lv_draw_rect_dsc_init(&dsc);
  dsc.bg_color = lv_color_hex(color);
  dsc.bg_opa = LV_OPA_COVER;
  dsc.radius = r;

  lv_area_t area = {x, y, x + w - 1, y + h - 1};
  lv_draw_rect(layer, &dsc, &area);

  if (r <= 0 || h <= r) return;
  dsc.radius = 0;
  lv_area_t foot = {x, y + h - r, x + w - 1, y + h - 1};
  lv_draw_rect(layer, &dsc, &foot);
}

void line(lv_layer_t* layer, int32_t x1, int32_t y1, int32_t x2, int32_t y2,
          uint32_t color, int32_t width, bool rounded, int32_t dash_width,
          int32_t dash_gap) {
  lv_draw_line_dsc_t dsc;
  lv_draw_line_dsc_init(&dsc);
  dsc.color = lv_color_hex(color);
  dsc.width = width;
  dsc.opa = LV_OPA_COVER;
  dsc.round_start = rounded;
  dsc.round_end = rounded;
  dsc.dash_width = dash_width;
  dsc.dash_gap = dash_gap;
  dsc.p1.x = x1;
  dsc.p1.y = y1;
  dsc.p2.x = x2;
  dsc.p2.y = y2;
  lv_draw_line(layer, &dsc);
}

void text(lv_layer_t* layer, int32_t x, int32_t y, int32_t w, const char* value,
          const lv_font_t* font, uint32_t color, lv_text_align_t align) {
  lv_draw_label_dsc_t dsc;
  lv_draw_label_dsc_init(&dsc);
  dsc.text = value;
  dsc.color = lv_color_hex(color);
  dsc.font = font;
  dsc.align = align;
  dsc.opa = LV_OPA_COVER;

  lv_area_t area = {x, y, x + w - 1, y + font->line_height};
  lv_draw_label(layer, &dsc, &area);
}

}  // namespace draw
