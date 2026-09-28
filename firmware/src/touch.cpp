#include "touch.h"

#include <Arduino.h>
#include <Wire.h>
#include <lvgl.h>

#include "board_config.h"

namespace touch {
namespace {

/**
 * AXS15231B touch, read over I2C at 0x3B.
 *
 * VERIFY THIS AGAINST THE WORKING SKETCH. The command word and the bit layout
 * below are what the JC3248W535 examples use, but touch protocols on these
 * panels vary between batches, and a wrong bit mask here looks like "the
 * screen ignores the bottom half" rather than like an error.
 */
constexpr uint8_t READ_COMMAND[8] = {0xB5, 0xAB, 0xA5, 0x5A, 0x00, 0x00, 0x00, 0x08};

struct Point {
  bool pressed;
  int16_t x;
  int16_t y;
};

Point read_panel() {
  uint8_t buf[8] = {0};

  Wire.beginTransmission(board::TOUCH_ADDR);
  Wire.write(READ_COMMAND, sizeof(READ_COMMAND));
  if (Wire.endTransmission() != 0) return {false, 0, 0};

  if (Wire.requestFrom(board::TOUCH_ADDR, sizeof(buf)) != sizeof(buf)) {
    return {false, 0, 0};
  }
  for (uint8_t &byte : buf) byte = Wire.read();

  const uint8_t fingers = buf[1];
  if (fingers == 0 || fingers > 5) return {false, 0, 0};

  // Native panel coordinates: 320 wide, 480 tall.
  const int16_t native_x = static_cast<int16_t>(((buf[2] & 0x0F) << 8) | buf[3]);
  const int16_t native_y = static_cast<int16_t>(((buf[4] & 0x0F) << 8) | buf[5]);

  // The panel is mounted portrait and the UI is landscape, so the axes swap.
  // If drags run backwards, flip the sign on one of these two lines.
  const int16_t x = native_y;
  const int16_t y = board::PANEL_NATIVE_WIDTH - 1 - native_x;

  if (x < 0 || x >= board::SCREEN_WIDTH || y < 0 || y >= board::SCREEN_HEIGHT) {
    return {false, 0, 0};
  }
  return {true, x, y};
}

void read_cb(lv_indev_t *, lv_indev_data_t *data) {
  // Held between reports so LVGL sees a continuous drag rather than a jump
  // back to the origin on the first dropped sample.
  static int16_t last_x = 0;
  static int16_t last_y = 0;

  const Point point = read_panel();
  if (point.pressed) {
    last_x = point.x;
    last_y = point.y;
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
  data->point.x = last_x;
  data->point.y = last_y;
}

}  // namespace

void begin() {
  if (board::TOUCH_RST >= 0) {
    pinMode(board::TOUCH_RST, OUTPUT);
    digitalWrite(board::TOUCH_RST, LOW);
    delay(10);
    digitalWrite(board::TOUCH_RST, HIGH);
    delay(50);
  }
  pinMode(board::TOUCH_INT, INPUT);

  Wire.begin(board::TOUCH_SDA, board::TOUCH_SCL, 400000);

  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, read_cb);
}

}  // namespace touch
