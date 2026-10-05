#pragma once
#include "../Screen.h"

namespace ui {

/** The DSMR 5.0 frame, trimmed to the lines worth reading. */
class TelegramScreen : public Screen {
 public:
  void update(const MeterSnapshot& snapshot) override;
  const char* name() const override { return "P1 telegram"; }

 protected:
  void build(lv_obj_t* root) override;

 private:
  static constexpr size_t MAX_LINES = 10;

  lv_obj_t* title_ = nullptr;
  lv_obj_t* codes_[MAX_LINES] = {nullptr};
  lv_obj_t* glosses_[MAX_LINES] = {nullptr};
};

}  // namespace ui
