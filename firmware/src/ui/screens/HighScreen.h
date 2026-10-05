#pragma once
#include "../Screen.h"
#include "../widgets/YearLine.h"
#include "InsightHeader.h"

namespace ui {

/** Twelve months, January first. Both years render from this one class. */
class HighScreen : public Screen {
 public:
  enum class Period { Current, Previous };
  explicit HighScreen(Period period) : period_(period) {}

  void update(const MeterSnapshot& snapshot) override;
  const char* name() const override {
    return period_ == Period::Current ? "High" : "High - previous";
  }

 protected:
  void build(lv_obj_t* root) override;

 private:
  const YearInsight& select(const MeterSnapshot& s) const {
    return period_ == Period::Current ? s.high : s.high_prev;
  }

  Period period_;
  InsightHeader header_;
  YearLine line_;
  lv_obj_t* unit_ = nullptr;
  lv_obj_t* total_ = nullptr;
};

}  // namespace ui
