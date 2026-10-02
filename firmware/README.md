# PPC Smart Meter Display — panel firmware

Two panels, two targets, one source tree. Only the selected board's driver is
compiled, so neither panel can break the other's build.

| Target | Panel | Controller | Touch |
| --- | --- | --- | --- |
| `panel-35` | 3.5" Gugxiom / Guition, 480 × 320 | AXS15231B over QSPI | AXS, I²C 0x3B |
| `panel-70` | 7.0" Elecrow CrowPanel DIS08070H, 800 × 480 | 16-bit RGB via LCD_CAM | GT911, I²C 0x5D |

```bash
cd firmware

pio run -e panel-35 -t upload && pio device monitor -e panel-35   # 3.5 inch
pio run -e panel-70 -t upload && pio device monitor -e panel-70   # 7.0 inch
```

Each target has its own flash size, partition table and PSRAM mode, so always
pass `-e`. `pio run` with no `-e` builds both and will flash neither.

> **Nothing here has been compiled.** There is no PlatformIO on the machine
> this was written on. Expect the first build of each target to need small
> fixes — most likely LVGL v9 descriptor field names, and the one pin-map
> question called out below.

## The two panels are not the same kind of device

The 3.5" is a **bus**: you push pixels at it and it holds them. Arduino_GFX
owns that bus and LVGL hands it finished stripes. Two partial draw buffers in
internal DMA-capable RAM; `LV_DISPLAY_RENDER_MODE_PARTIAL`.

The 7" is a **stream**: the LCD_CAM peripheral reads the framebuffer out to the
panel continuously, for ever, whether or not anything changed. There is no
"push". Everything that goes wrong on it is a question of whether that stream
keeps up. That is why it does not go through Arduino_GFX — the knobs that
matter live in the ESP-IDF RGB driver and nowhere else.

## Tuning the 7 inch panel

### Stripes and flicker down the left edge

This is the RGB DMA running dry, and it is a bandwidth problem, not a timing
one. 800 × 480 × 2 bytes is 768,000 bytes — 750 KiB — which only fits in PSRAM. The peripheral
reads that framebuffer continuously while the CPU, the cache and WiFi want the
same memory. Lose the race and the FIFO empties mid-line; the panel keeps
clocking and latches whatever is on the bus. It appears at the start of lines —
the left edge — and it moves, which is what makes it read as flicker rather
than as a fixed artefact.

Work through these in order. The first one is usually the whole fix.

1. **Bounce buffers.** `BOUNCE_LINES` in `src/board/Panel70.cpp`, currently 10
   (15 KiB each, 30 KiB of internal RAM across both). DMA then reads from internal SRAM and a
   driver task refills it from PSRAM in bursts, which is what PSRAM is good at.
   If stripes survive, raise to 16 or 20 before touching anything else.
2. **PSRAM at 80 MHz, octal.** Already set in `platformio.ini`
   (`memory_type = qio_opi`, `CONFIG_SPIRAM_SPEED_80M`). Worth confirming the
   module really is an N4R8 — a quad-PSRAM part cannot feed this panel.
3. **Drop the pixel clock.** `PCLK_HZ` in `src/board/pins_70.h`, currently
   15 MHz. 14 or 12 MHz buys headroom directly. See the trade-off below.
4. **Keep flash quiet while the panel runs.** Any flash write stalls the cache
   and starves the DMA for milliseconds. If you add NVS or OTA, expect a visible
   tear at exactly that moment.

### Lag

The cause is almost always a full-frame copy. With two framebuffers and
`LV_DISPLAY_RENDER_MODE_FULL`, LVGL renders straight into one of the panel's
own buffers and the flush is a swap, not a 750 KiB memcpy through PSRAM. That is
how `Panel70::begin` is set up; if you change `num_fbs` to 1 or switch to
`PARTIAL`, the copy comes back and so does the lag.

### The refresh rate is low, and that is deliberate

With the timings in `pins_70.h` the frame is 901 × 525 = 473,025 pixel clocks.
At 15 MHz that is **31.7 Hz**. Some people see that as a shimmer on a white
screen — which this design is.

Raising the clock raises refresh and PSRAM bandwidth together:

| Pixel clock | Refresh | Bandwidth |
| --- | --- | --- |
| 12 MHz | 25.4 Hz | 24 MB/s |
| 15 MHz (current) | 31.7 Hz | 30 MB/s |
| 18 MHz | 38.1 Hz | 36 MB/s |
| 21 MHz | 44.4 Hz | 42 MB/s |

If you go above 15 MHz, raise `BOUNCE_LINES` in the same change. Going up
without it is the single most reliable way to reproduce the stripes.

### Validating the RGB mapping

The pin map in `pins_70.h` is cross-checked against Elecrow's wiki and a
working ESPHome config. They agree on every control line, the backlight and
the touch bus — **and they disagree on the order of the red data lines.** Blue
and green match exactly; red does not. The file follows the ESPHome ordering,
because that is a configuration people are running rather than a table someone
transcribed, and keeps the other as a commented alternative.

Settle it on hardware rather than by reading:

```bash
pio run -e panel-70 -t upload --build-flag="-DRGB_TEST_PATTERN"
```

The panel draws colour bars across the top half and a per-channel ramp across
the bottom, instead of the UI.

- **Bars pure, ramps smooth** → the mapping is right.
- **A ramp that steps unevenly or doubles back** → that channel's bits are in
  the wrong order. For red, swap to the commented `LCD_R*` line.
- **Bars in the wrong order, or red and blue swapped** → whole channels are
  transposed in `data_gpio_nums`.
- **Vertical shimmer on every edge** → `PCLK_ACTIVE_NEG` is wrong. This panel
  latches on the falling edge.

Do this before judging anything about the UI. A wrong pin map looks like a
rendering bug and is not one.

## Known gaps

- **The horizontal swipe is backwards.** `lv_tileview` scrolls natively —
  dragging left reveals the tile on the *right*. The design, and the web app,
  do the opposite. Fixing it means taking the gesture off tileview and driving
  `lv_obj_set_tile_id` from our own `LV_EVENT_PRESSING` handler.
- **Only Home is a real screen.** The other nine tiles are
  `PlaceholderScreen` cards so the deck can be navigated from the first build.
- **Fonts are Montserrat**, LVGL's built-ins. The design wants Source Sans 3,
  and the 7" target wants a 72pt face that Montserrat does not ship. Convert
  with `lv_font_conv`; `data/SampleData.h` carries the exact glyph subset in a
  comment so the conversion need not include all of Latin-1. Flash is 4MB on
  the 7" board, so that subset matters.
- **Touch is unvalidated on both panels.** The AXS read sequence and the GT911
  address probe are both written from documentation, not from a scope.

## Layout

```
src/
  main.cpp                 instantiates App
  app/App                  owns the panel, the touch and the deck; runs LVGL
  board/
    Board.h                picks the pair for this target — the only #if
    Metrics.h              one design space, two panels: px() scales it
    Panel35 / TouchAXS     3.5 inch, Arduino_GFX + AXS touch
    Panel70 / TouchGT911   7.0 inch, esp_lcd RGB + GT911
    pins_35.h / pins_70.h  pin maps and panel timing
  ui/                      screens, widgets, theme — identical for both targets
  data/                    GENERATED from the web app by npm run gen:firmware
```

Both panels are **320 design-pixels tall** — the small one natively, the big
one at 1.5× — so one vertical rhythm serves both and `metrics::px()` is the
only thing that differs. The web app in the repo root works the same way.
