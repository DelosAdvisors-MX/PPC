#pragma once
#include <lvgl.h>
#include <stdint.h>

namespace board {

/** The AXS15231B capacitive panel, read over I2C and fed to LVGL. */
class TouchAXS {
 public:
  TouchAXS() = default;
  TouchAXS(const TouchAXS&) = delete;
  TouchAXS& operator=(const TouchAXS&) = delete;

  bool begin();

 private:
  struct Point {
    bool pressed;
    int16_t x;
    int16_t y;
  };

  static void read_trampoline(lv_indev_t* indev, lv_indev_data_t* data);
  void read(lv_indev_data_t* data);
  Point read_panel();

  lv_indev_t* indev_ = nullptr;
  // Held between reports so LVGL sees a continuous drag rather than a jump
  // back to the origin the first time a sample is dropped.
  int16_t last_x_ = 0;
  int16_t last_y_ = 0;
};

}  // namespace board
