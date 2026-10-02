#include "TouchAXS.h"

#include <Arduino.h>
#include <Wire.h>

#include "Metrics.h"
#include "pins_35.h"

namespace board {
namespace {

/**
 * VERIFY AGAINST THE WORKING SKETCH.
 *
 * The command word and bit layout below are what the JC3248W535 examples use,
 * but touch protocols on these panels vary between batches. A wrong mask here
 * does not look like an error — it looks like the screen ignoring half of
 * itself.
 */
constexpr uint8_t READ_COMMAND[8] = {0xB5, 0xAB, 0xA5, 0x5A, 0x00, 0x00, 0x00, 0x08};

}  // namespace

TouchAXS::Point TouchAXS::read_panel() {
  uint8_t buf[8] = {0};

  Wire.beginTransmission(pins35::TOUCH_ADDR);
  Wire.write(READ_COMMAND, sizeof(READ_COMMAND));
  if (Wire.endTransmission() != 0) return {false, 0, 0};
  if (Wire.requestFrom(pins35::TOUCH_ADDR, sizeof(buf)) != sizeof(buf)) return {false, 0, 0};
  for (uint8_t& byte : buf) byte = Wire.read();

  const uint8_t fingers = buf[1];
  if (fingers == 0 || fingers > 5) return {false, 0, 0};

  // Native panel coordinates: 320 wide, 480 tall.
  const int16_t native_x = static_cast<int16_t>(((buf[2] & 0x0F) << 8) | buf[3]);
  const int16_t native_y = static_cast<int16_t>(((buf[4] & 0x0F) << 8) | buf[5]);

  // The panel is mounted portrait and the UI is landscape, so the axes swap.
  // If drags run backwards, flip the sign on one of these two lines.
  const int16_t x = native_y;
  const int16_t y = pins35::PANEL_NATIVE_WIDTH - 1 - native_x;

  if (x < 0 || x >= pins35::SCREEN_WIDTH || y < 0 || y >= pins35::SCREEN_HEIGHT) {
    return {false, 0, 0};
  }
  return {true, x, y};
}

void TouchAXS::read_trampoline(lv_indev_t* indev, lv_indev_data_t* data) {
  static_cast<TouchAXS*>(lv_indev_get_user_data(indev))->read(data);
}

void TouchAXS::read(lv_indev_data_t* data) {
  const Point point = read_panel();
  if (point.pressed) {
    last_x_ = point.x;
    last_y_ = point.y;
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
  data->point.x = last_x_;
  data->point.y = last_y_;
}

bool TouchAXS::begin() {
  if (pins35::TOUCH_RST >= 0) {
    pinMode(pins35::TOUCH_RST, OUTPUT);
    digitalWrite(pins35::TOUCH_RST, LOW);
    delay(10);
    digitalWrite(pins35::TOUCH_RST, HIGH);
    delay(50);
  }
  pinMode(pins35::TOUCH_INT, INPUT);
  Wire.begin(pins35::TOUCH_SDA, pins35::TOUCH_SCL, 400000);

  indev_ = lv_indev_create();
  lv_indev_set_type(indev_, LV_INDEV_TYPE_POINTER);
  lv_indev_set_user_data(indev_, this);
  lv_indev_set_read_cb(indev_, read_trampoline);
  return true;
}

}  // namespace board
