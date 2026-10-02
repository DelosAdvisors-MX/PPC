#pragma once
#include <lvgl.h>
#include <stdint.h>

namespace board {

/** GT911 capacitive touch, as the 7 inch CrowPanel carries it. */
class TouchGT911 {
 public:
  TouchGT911() = default;
  TouchGT911(const TouchGT911&) = delete;
  TouchGT911& operator=(const TouchGT911&) = delete;

  bool begin();

 private:
  static void read_trampoline(lv_indev_t* indev, lv_indev_data_t* data);
  void read(lv_indev_data_t* data);
  bool read_point(int16_t& x, int16_t& y);

  bool write_register(uint16_t reg, uint8_t value);
  bool read_registers(uint16_t reg, uint8_t* out, size_t length);

  lv_indev_t* indev_ = nullptr;
  uint8_t address_ = 0;
  int16_t last_x_ = 0;
  int16_t last_y_ = 0;
};

}  // namespace board
