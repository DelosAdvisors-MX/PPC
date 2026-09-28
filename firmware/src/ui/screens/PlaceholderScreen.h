#pragma once
#include <stdint.h>

#include "../Screen.h"

namespace ui {

/**
 * A named card standing in for a screen that has not been ported yet.
 *
 * It exists so the deck can be navigated end to end from the first build —
 * gestures and the screen map are worth proving before any of the drawing is.
 */
class PlaceholderScreen : public Screen {
 public:
  PlaceholderScreen(const char* name, uint32_t accent) : name_(name), accent_(accent) {}

  void update(const MeterSnapshot&) override {}
  const char* name() const override { return name_; }

 protected:
  void build(lv_obj_t* root) override;

 private:
  const char* name_;
  uint32_t accent_;
};

}  // namespace ui
