#pragma once
#include "ChartWidget.h"

namespace ui {

/** This home against the comparison group's average — two bars, one axis. */
class CompareBars : public ChartWidget {
 public:
  void set_pair(float mine, float theirs);

 protected:
  void draw(lv_layer_t* layer) override;

 private:
  float mine_ = 0.0f;
  float theirs_ = 0.0f;
};

}  // namespace ui
