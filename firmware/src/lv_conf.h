#pragma once

// Only the settings that differ from LVGL's defaults. Everything else falls
// through to lv_conf_internal.h, so this file stays short enough to read.

#define LV_COLOR_DEPTH 16

// FreeRTOS-aware LVGL. This is what makes lv_lock()/lv_unlock() real; without
// it they compile to nothing and every lv_* call from a second task is a race.
#define LV_USE_OS LV_OS_FREERTOS

// Draw buffers live in internal DMA-capable RAM, allocated in display.cpp.
#define LV_USE_DRAW_SW 1
#define LV_DRAW_SW_COMPLEX 1

// millis() drives the tick; see main.cpp.
#define LV_TICK_CUSTOM 0

// Built-ins to get the first build on screen. The real type scale is generated
// from the web app's tokens and replaces these — see README.
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_20 1
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_MONTSERRAT_34 1
#define LV_FONT_MONTSERRAT_48 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14

// The deck is an lv_tileview; the charts are custom draw callbacks.
#define LV_USE_TILEVIEW 1
#define LV_USE_ARC 1
#define LV_USE_LABEL 1
#define LV_USE_IMAGE 1

#define LV_USE_LOG 1
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN
