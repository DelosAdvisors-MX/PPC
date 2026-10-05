#pragma once
#include "../Screen.h"
#include "../widgets/ApplianceGlyph.h"

namespace ui {

/** Consumption by machine, so far this year. No previous-period view. */
class AppliancesScreen : public Screen {
 public:
  void update(const MeterSnapshot& snapshot) override;
  const char* name() const override { return "Appliances"; }

 protected:
  void build(lv_obj_t* root) override;

 private:
  static constexpr size_t MAX_ROWS = 5;

  struct Row {
    ApplianceGlyph glyph;
    lv_obj_t* name = nullptr;
    lv_obj_t* track = nullptr;
    lv_obj_t* fill = nullptr;
    lv_obj_t* share = nullptr;
  };

  lv_obj_t* headline_ = nullptr;
  lv_obj_t* intro_ = nullptr;
  Row rows_[MAX_ROWS];
};

}  // namespace ui
