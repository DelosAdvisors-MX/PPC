#pragma once
#include <stddef.h>
#include <stdint.h>

/**
 * The reading every screen draws from. Mirrors src/lib/types.ts in the web
 * app, which is the contract both ends agree on.
 *
 * Plain structs, no methods: these live in flash as `const` today and will be
 * filled from the P1 port later. Keeping them trivially copyable means a
 * parser task can build one and hand it over whole under a single lock.
 */

enum class ApplianceIcon : uint8_t { HotWater, TumbleDryer, Oven, Hob, Lighting };

struct LiveReading {
  float kw;
  float scale_kw;
  const char* caption;
};

/** What a closed month shows in place of the live dial. */
struct PeriodAverages {
  float kw;
  float kwh_per_day;
};

struct MonthProgress {
  const char* month;
  float estimate_kwh;
  float used_kwh;
  const char* caption;
  bool closed;
};

struct HomeData {
  const char* title;
  const char* badge;        // nullptr on the live screen
  const LiveReading* live;  // nullptr on a closed month
  const PeriodAverages* averages;  // nullptr on the live screen
  MonthProgress progress;
};

struct Appliance {
  const char* name;
  ApplianceIcon icon;
  float kwh;
  float share;  // percent of the year
};

struct AppliancesData {
  const char* headline;
  const char* intro;
  const Appliance* items;
  size_t count;
};

struct DailyInsight {
  const char* title;
  const float* daily;
  size_t days;
};

struct YearInsight {
  const char* title;
  const float* monthly;  // always twelve, January first
};

struct CompareData {
  const char* eyebrow;  // nullptr on the current-period screen
  const char* subtitle;
  const char* unit_label;
  float my_kwh;
  float their_kwh;
  const char* const* chips;
  size_t chip_count;
  uint32_t group_size;
};

struct TelegramLine {
  const char* code;
  const char* gloss;
};

struct TelegramData {
  const TelegramLine* lines;
  size_t count;
  const char* highlight;  // the `code` drawn in mint
};

struct MeterSnapshot {
  HomeData home;
  HomeData home_prev;
  AppliancesData appliances;
  DailyInsight medium;
  DailyInsight medium_prev;
  YearInsight high;
  YearInsight high_prev;
  CompareData compare;
  CompareData compare_prev;
  TelegramData telegram;
};
