#pragma once
#include "../Screen.h"
#include "../widgets/FillBar.h"
#include "../widgets/Gauge.h"

namespace ui {

/**
 * The entry screen: instantaneous draw on the left, progress against the
 * month's estimate on the right.
 *
 * One class renders both periods. A closed month drops the dial — a needle
 * pointing at a live reading means nothing once the month is over — and shows
 * what the month averaged instead.
 */
class HomeScreen : public Screen {
 public:
  enum class Period { Current, Previous };

  explicit HomeScreen(Period period) : period_(period) {}

  void update(const MeterSnapshot& snapshot) override;
  const char* name() const override {
    return period_ == Period::Current ? "Home" : "Home - previous";
  }

 protected:
  void build(lv_obj_t* root) override;

 private:
  const HomeData& select(const MeterSnapshot& snapshot) const {
    return period_ == Period::Current ? snapshot.home : snapshot.home_prev;
  }

  Period period_;

  Gauge gauge_;
  FillBar fill_;

  lv_obj_t* title_ = nullptr;
  lv_obj_t* badge_ = nullptr;
  lv_obj_t* reading_ = nullptr;
  lv_obj_t* reading_caption_ = nullptr;
  lv_obj_t* per_day_ = nullptr;
  lv_obj_t* per_day_caption_ = nullptr;
  lv_obj_t* divider_ = nullptr;
  lv_obj_t* estimate_ = nullptr;
  lv_obj_t* progress_caption_ = nullptr;
};

}  // namespace ui
