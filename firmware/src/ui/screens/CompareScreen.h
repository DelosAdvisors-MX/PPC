#pragma once
#include "../Screen.h"
#include "../widgets/CompareBars.h"

namespace ui {

/** This home against similar homes. Both periods render from one class. */
class CompareScreen : public Screen {
 public:
  enum class Period { Current, Previous };
  explicit CompareScreen(Period period) : period_(period) {}

  void update(const MeterSnapshot& snapshot) override;
  const char* name() const override {
    return period_ == Period::Current ? "Compare" : "Compare - previous";
  }

 protected:
  void build(lv_obj_t* root) override;

 private:
  static constexpr size_t MAX_CHIPS = 4;

  const CompareData& select(const MeterSnapshot& s) const {
    return period_ == Period::Current ? s.compare : s.compare_prev;
  }

  Period period_;
  CompareBars bars_;
  lv_obj_t* eyebrow_ = nullptr;
  lv_obj_t* percent_ = nullptr;
  lv_obj_t* direction_ = nullptr;
  lv_obj_t* subtitle_ = nullptr;
  lv_obj_t* chips_[MAX_CHIPS] = {nullptr, nullptr, nullptr, nullptr};
  lv_obj_t* chip_labels_[MAX_CHIPS] = {nullptr, nullptr, nullptr, nullptr};
  lv_obj_t* group_ = nullptr;
  lv_obj_t* unit_ = nullptr;
};

}  // namespace ui
