#pragma once
#include <stdint.h>

/**
 * Elecrow CrowPanel DIS08070H — 7.0 inch, ESP32-S3-WROOM-1-N4R8.
 *
 * Pin map cross-checked against Elecrow's own wiki and a working ESPHome
 * configuration for this board; both agree on every control line, the
 * backlight and the touch bus.
 *
 * ONE DISAGREEMENT, and it is the one worth knowing about: the two sources
 * order the red data lines differently. Blue and green agree exactly; red does
 * not. The values below follow the ESPHome config, because that is a
 * configuration people are running rather than a table someone transcribed.
 *
 * If red comes out wrong on hardware — reds reading as dark, or a colour ramp
 * that steps unevenly — try the alternative below. Build with -DRGB_TEST_PATTERN
 * and the panel draws colour bars and ramps instead of the UI, which settles it
 * in about ten seconds. See README "Validating the RGB mapping".
 */
namespace pins70 {

// Control
constexpr int LCD_DE = 41;
constexpr int LCD_VSYNC = 40;
constexpr int LCD_HSYNC = 39;
constexpr int LCD_PCLK = 0;
constexpr int LCD_BACKLIGHT = 2;

// 16-bit RGB565 bus, LSB first within each channel, as esp_lcd expects:
// data_gpio_nums[0..4] = B0..B4, [5..10] = G0..G5, [11..15] = R0..R4.
constexpr int LCD_B0 = 4, LCD_B1 = 5, LCD_B2 = 6, LCD_B3 = 7, LCD_B4 = 15;
constexpr int LCD_G0 = 1, LCD_G1 = 16, LCD_G2 = 8, LCD_G3 = 3, LCD_G4 = 46, LCD_G5 = 9;
constexpr int LCD_R0 = 45, LCD_R1 = 48, LCD_R2 = 47, LCD_R3 = 21, LCD_R4 = 14;

/** The other published red ordering, if the one above looks wrong. */
// constexpr int LCD_R0 = 45, LCD_R1 = 14, LCD_R2 = 21, LCD_R3 = 47, LCD_R4 = 48;

// GT911 capacitive touch. 0x5D is the usual address; some units answer on 0x14,
// which is decided by the INT pin level while the controller comes out of reset.
constexpr int TOUCH_SDA = 19;
constexpr int TOUCH_SCL = 20;
constexpr int TOUCH_INT = -1;  // not broken out on this board
constexpr int TOUCH_RST = -1;
constexpr uint8_t TOUCH_ADDR_PRIMARY = 0x5D;
constexpr uint8_t TOUCH_ADDR_ALTERNATE = 0x14;

/**
 * Panel timing.
 *
 * 801 x 525 total against 800 x 480 visible, so a frame is 473'025 pixel
 * clocks. At 15 MHz that is 31,7 Hz — low, and part of why this panel can look
 * like it shimmers. Raising the clock raises the refresh rate and the PSRAM
 * bandwidth in the same breath; read the README before touching it.
 */
constexpr uint32_t PCLK_HZ = 15 * 1000 * 1000;
constexpr int H_RES = 800;
constexpr int V_RES = 480;
constexpr int HSYNC_FRONT_PORCH = 40;
constexpr int HSYNC_PULSE_WIDTH = 48;
constexpr int HSYNC_BACK_PORCH = 13;
constexpr int VSYNC_FRONT_PORCH = 1;
constexpr int VSYNC_PULSE_WIDTH = 31;
constexpr int VSYNC_BACK_PORCH = 13;
/** This panel latches on the falling edge. Getting it wrong shimmers columns. */
constexpr bool PCLK_ACTIVE_NEG = true;

}  // namespace pins70
