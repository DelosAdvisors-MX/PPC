#pragma once
#include "../Screen.h"
#include "../widgets/DailyBars.h"
#include "InsightHeader.h"

namespace ui {

/** One month, day by day. Both periods render from this one class. */
class MediumScreen : public Screen {
 public:
  enum class Period { Current, Previous };
  explicit MediumScreen(Period period) : period_(period) {}

  void update(const MeterSnapshot& snapshot) override;
  const char* name() const override {
    return period_ == Period::Current ? "Medium" : "Medium - previous";
  }

 protected:
  void build(lv_obj_t* root) override;

 private:
  const DailyInsight& select(const MeterSnapshot& s) const {
    return period_ == Period::Current ? s.medium : s.medium_prev;
  }

  Period period_;
  InsightHeader header_;
  DailyBars bars_;
  lv_obj_t* unit_ = nullptr;
  lv_obj_t* legend_ = nullptr;
};

}  // namespace ui
