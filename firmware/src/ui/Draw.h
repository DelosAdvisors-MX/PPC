#pragma once
#include <lvgl.h>
#include <stdint.h>

/** Small drawing helpers shared by the chart widgets. */
namespace draw {

/**
 * A bar rounded on top and square where it meets the axis.
 *
 * LVGL's rect radius applies to all four corners, so the bar is drawn rounded
 * and its foot is then covered with a square-cornered rect of the same
 * colour. This is the same shape the web app draws as a path, and it is there
 * for the same reason: a bar with a rounded foot reads as hanging below zero.
 */
void top_rounded_bar(lv_layer_t* layer, int32_t x, int32_t y, int32_t w, int32_t h,
                     int32_t radius, uint32_t color);

/** A 1px line. `width` thickens it; `rounded` caps the ends. */
void line(lv_layer_t* layer, int32_t x1, int32_t y1, int32_t x2, int32_t y2,
          uint32_t color, int32_t width = 1, bool rounded = false,
          int32_t dash_width = 0, int32_t dash_gap = 0);

/** A plain filled rectangle. */
void rect(lv_layer_t* layer, int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color,
          int32_t radius = 0);

/** A filled circle, centred, optionally ringed. */
void circle(lv_layer_t* layer, int32_t cx, int32_t cy, int32_t r, uint32_t color,
            uint32_t ring_color = 0, int32_t ring_width = 0);

/** Text anchored by `align` inside a box of the given width. */
void text(lv_layer_t* layer, int32_t x, int32_t y, int32_t w, const char* value,
          const lv_font_t* font, uint32_t color, lv_text_align_t align = LV_TEXT_ALIGN_LEFT);

/** Text with a halo behind it, for a label that may land on the line it names. */
void halo_text(lv_layer_t* layer, int32_t x, int32_t y, int32_t w, const char* value,
               const lv_font_t* font, uint32_t color, uint32_t halo,
               lv_text_align_t align = LV_TEXT_ALIGN_LEFT);

}  // namespace draw
