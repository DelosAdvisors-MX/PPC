#include "TouchGT911.h"

#include <Arduino.h>
#include <Wire.h>

#include "Metrics.h"
#include "pins_70.h"

namespace board {
namespace {

using namespace pins70;

/** Coordinates and the flag saying a frame of them is ready. */
constexpr uint16_t REG_STATUS = 0x814E;
constexpr uint16_t REG_POINT_1 = 0x8150;

constexpr uint8_t STATUS_READY = 0x80;
constexpr uint8_t STATUS_COUNT_MASK = 0x0F;

}  // namespace

bool TouchGT911::write_register(uint16_t reg, uint8_t value) {
  Wire.beginTransmission(address_);
  Wire.write(static_cast<uint8_t>(reg >> 8));
  Wire.write(static_cast<uint8_t>(reg & 0xFF));
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool TouchGT911::read_registers(uint16_t reg, uint8_t* out, size_t length) {
  Wire.beginTransmission(address_);
  Wire.write(static_cast<uint8_t>(reg >> 8));
  Wire.write(static_cast<uint8_t>(reg & 0xFF));
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(address_, length) != length) return false;
  for (size_t i = 0; i < length; i++) out[i] = Wire.read();
  return true;
}

bool TouchGT911::read_point(int16_t& x, int16_t& y) {
  uint8_t status = 0;
  if (!read_registers(REG_STATUS, &status, 1)) return false;
  if ((status & STATUS_READY) == 0) return false;

  const uint8_t count = status & STATUS_COUNT_MASK;
  bool pressed = false;

  if (count > 0) {
    uint8_t point[8] = {0};
    if (read_registers(REG_POINT_1, point, sizeof(point))) {
      x = static_cast<int16_t>(point[1] | (point[2] << 8));
      y = static_cast<int16_t>(point[3] | (point[4] << 8));
      pressed = x >= 0 && x < metrics::WIDTH && y >= 0 && y < metrics::HEIGHT;
    }
  }

  // The controller holds the frame until the flag is cleared; leave it set and
  // every later read returns the same stale coordinates.
  write_register(REG_STATUS, 0);
  return pressed;
}

void TouchGT911::read_trampoline(lv_indev_t* indev, lv_indev_data_t* data) {
  static_cast<TouchGT911*>(lv_indev_get_user_data(indev))->read(data);
}

void TouchGT911::read(lv_indev_data_t* data) {
  int16_t x = 0;
  int16_t y = 0;
  if (read_point(x, y)) {
    last_x_ = x;
    last_y_ = y;
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
  // Held between reports so a dropped sample does not read as a jump home.
  data->point.x = last_x_;
  data->point.y = last_y_;
}

bool TouchGT911::begin() {
  if (TOUCH_RST >= 0) {
    pinMode(TOUCH_RST, OUTPUT);
    digitalWrite(TOUCH_RST, LOW);
    delay(10);
    digitalWrite(TOUCH_RST, HIGH);
    delay(60);
  }
  Wire.begin(TOUCH_SDA, TOUCH_SCL, 400000);

  // Which address the controller answers on is decided while it leaves reset,
  // and this board does not break out the pin that chooses. Ask both.
  for (uint8_t candidate : {TOUCH_ADDR_PRIMARY, TOUCH_ADDR_ALTERNATE}) {
    Wire.beginTransmission(candidate);
    if (Wire.endTransmission() == 0) {
      address_ = candidate;
      break;
    }
  }
  if (address_ == 0) {
    Serial.println("TouchGT911: no controller on 0x5D or 0x14");
    return false;
  }
  Serial.printf("TouchGT911: found at 0x%02X\n", address_);

  indev_ = lv_indev_create();
  lv_indev_set_type(indev_, LV_INDEV_TYPE_POINTER);
  lv_indev_set_user_data(indev_, this);
  lv_indev_set_read_cb(indev_, read_trampoline);
  return true;
}

}  // namespace board
