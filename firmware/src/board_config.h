#pragma once

// Pin map for the AXS15231B QSPI panel. These came off the working sketch —
// change them here and nowhere else.
namespace board {

// Display, QSPI
constexpr int LCD_CS = 45;
constexpr int LCD_CLK = 47;
constexpr int LCD_D0 = 21;
constexpr int LCD_D1 = 48;
constexpr int LCD_D2 = 40;
constexpr int LCD_D3 = 39;
constexpr int LCD_RST = -1;  // tied high on this board; set if yours has one
constexpr int LCD_BACKLIGHT = 1;

// Touch, I2C
constexpr int TOUCH_SDA = 4;
constexpr int TOUCH_SCL = 8;
constexpr int TOUCH_RST = 12;
constexpr int TOUCH_INT = 11;
constexpr uint8_t TOUCH_ADDR = 0x3B;

// The panel is 320x480 portrait in hardware; the design is 480x320 landscape.
constexpr int PANEL_NATIVE_WIDTH = 320;
constexpr int PANEL_NATIVE_HEIGHT = 480;
constexpr int SCREEN_WIDTH = 480;
constexpr int SCREEN_HEIGHT = 320;
constexpr uint8_t ROTATION = 1;  // verify against the working sketch

}  // namespace board
