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

## How it is put together

Classes where they earn their keep, namespaces where they would be ceremony.

```
src/
  main.cpp                 instantiates App, nothing else
  app/App                  owns Display, TouchPanel and Deck; runs the LVGL task
  hal/Display              QSPI panel + LVGL's view of it
  hal/TouchPanel           AXS15231B over I2C, fed to LVGL as a pointer device
  ui/Deck                  the tileview and the ten screens it owns
  ui/Screen                abstract: mount(tile) once, update(snapshot) after
  ui/widgets/ChartWidget   abstract: one LVGL object whose draw is a virtual
  ui/widgets/Gauge         \
  ui/widgets/FillBar        > ports of the web app's inline SVG components
  ui/screens/HomeScreen    both periods from one class
  ui/Theme.h               GENERATED from src/lib/tokens.ts
  data/SampleData.h        GENERATED from src/lib/data.ts
```

Two rules worth keeping:

**Nothing happens in a constructor.** `App` is a global, and on Arduino globals
are constructed before `Serial` exists and before the heap has settled. A
failure there looks like a hardware fault. Everything real is in `begin()`.

**Screens and widgets are held by value.** LVGL objects hold raw pointers back
to them, so they must outlive their tiles. The Deck owns its screens as
members and each screen owns its widgets the same way — no heap, no lifetime
question.

The C callbacks reach their objects through static trampolines and LVGL's user
data (`lv_display_set_user_data`, `lv_indev_set_user_data`, the event's user
data), which is the only place the C and C++ halves touch.

## Generated from the web app

`npm run gen:firmware` (from the repo root) regenerates `ui/Theme.h` and
`data/SampleData.h` from `src/lib/tokens.ts` and `src/lib/data.ts`. Node strips
the types on import, so it reads the web app's real source rather than a copy
and the two cannot drift.

It also emits the exact glyph subset the UI needs, in a comment at the top of
`SampleData.h`, so `lv_font_conv` can be given that range and nothing more.

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
- [x] Skeleton: the tileview screen map, navigable end to end
- [x] Theme and sample data generated from the web app
- [x] Home, with the Gauge and FillBar widgets — the vertical slice that
      proves the widget pattern
- [ ] Real fonts: convert Source Sans 3 to the seven roles in `ui/Fonts.h`
- [ ] Remaining screens: Telegram, Appliances, Compare, Medium, High
- [ ] Data: P1 serial, or polling `/api/meter`

The other eight tiles are `PlaceholderScreen` cards so the deck can be
navigated from the first build. Gestures and the screen map are worth proving
before any of the drawing is.

The Next.js app in the repo root stays as the golden master — it renders every
screen pixel-exact in a browser, so "what should this look like" always has an
unambiguous answer.
