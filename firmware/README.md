# PPC Smart Meter Display — panel firmware

The design, running natively on the ESP32-S3 + AXS15231B QSPI panel.

**Arduino_GFX keeps the panel, LVGL takes the drawing.** The QSPI bus, the
AXS15231B init and the backlight stay exactly as the working sketch had them;
LVGL never talks to the bus, it just hands finished stripes to
`gfx->draw16bitRGBBitmap()` in `src/display.cpp`. The hard part is not
rewritten.

## Why not stay on Arduino_GFX alone

The three faults in the first build are all Arduino_GFX defaults, not bugs:

| Symptom | Cause |
| --- | --- |
| Chunky bitmap text | The built-in 5x7 glyph font, integer-scaled by `setTextSize()` |
| Bars with rounded feet below zero | `fillRoundRect()` rounds all four corners |
| Screens overlapping mid-swipe | No framebuffer, so partial redraws reach the panel |

LVGL fixes all three structurally: anti-aliased scalable fonts, arbitrary
shapes, and double-buffered partial rendering. It also brings `lv_tileview`,
which is the swipe deck almost for free.

## Build

```bash
cd firmware
pio run -t upload && pio device monitor
```

Board assumptions in `platformio.ini`: 16MB flash, 8MB octal PSRAM (N16R8),
native USB-Serial/JTAG. Adjust if the module differs.

## Verify in this order

The scaffold is uncompiled — nothing here has been on hardware yet. Three
things are guesses that the working sketch can settle:

1. **Rotation.** The panel is 320x480 portrait; the design is 480x320
   landscape. `board::ROTATION` is set to 1. If the AXS15231B cannot rotate in
   hardware, Arduino_GFX falls back to rotating in software, which is slow
   enough to matter on a full-screen swipe.
2. **Touch protocol.** `src/touch.cpp` uses the command word and bit layout the
   JC3248W535 examples use. A wrong mask here does not look like an error — it
   looks like the screen ignoring part of itself.
3. **Colour order.** `flush_cb` swaps RGB565 byte order. If colours come out
   inverted, that is the line to remove.

Then: a swipe across the deck with no tearing. That is the bar the first build
failed, and everything else is wasted until it passes.

## Where this is going

- [x] Bring-up: QSPI, LVGL, double DMA buffers, LVGL in its own task
- [x] Skeleton: the tileview screen map with placeholders
- [ ] Theme and fonts generated from the web app's `src/lib/tokens.ts`
- [ ] Screens: Telegram, Appliances, Home, Compare, Medium, High
- [ ] Data: P1 serial, or polling `/api/meter`

The Next.js app in the repo root stays as the golden master — it renders every
screen pixel-exact in a browser, so "what should this look like" always has an
unambiguous answer.
