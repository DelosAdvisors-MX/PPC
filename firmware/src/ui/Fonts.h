#pragma once
#include <lvgl.h>

/**
 * The type scale, by role rather than by size.
 *
 * The web design uses about fifteen distinct sizes. An LVGL font is a bitmap
 * per size in flash, so the scale is rationalised to seven here and the
 * screens ask for a role. When the real faces are converted — Source Sans 3,
 * or Ping LCG once licensed — only this file changes.
 *
 * Montserrat is a stand-in so the first build has something to draw.
 */
namespace fonts {

#if defined(PANEL_70)
// 1.5x the small panel, rounded to sizes LVGL ships. The reading would want
// 72pt; Montserrat stops at 48, so the biggest figure loses a little presence
// until a real face is converted.
inline const lv_font_t* reading() { return &lv_font_montserrat_48; }
inline const lv_font_t* headline() { return &lv_font_montserrat_48; }
inline const lv_font_t* title() { return &lv_font_montserrat_34; }
inline const lv_font_t* subtitle() { return &lv_font_montserrat_30; }
inline const lv_font_t* body() { return &lv_font_montserrat_24; }
inline const lv_font_t* caption() { return &lv_font_montserrat_20; }
inline const lv_font_t* micro() { return &lv_font_montserrat_18; }
#else
inline const lv_font_t* reading() { return &lv_font_montserrat_48; }   // 0,84
inline const lv_font_t* headline() { return &lv_font_montserrat_34; }  // 21,0 kWh
inline const lv_font_t* title() { return &lv_font_montserrat_24; }     // screen titles
inline const lv_font_t* subtitle() { return &lv_font_montserrat_20; }  // insight pills
inline const lv_font_t* body() { return &lv_font_montserrat_16; }      // list rows
inline const lv_font_t* caption() { return &lv_font_montserrat_14; }   // axis labels
inline const lv_font_t* micro() { return &lv_font_montserrat_12; }     // chips, glosses
#endif

}  // namespace fonts
