# PPC Smart Meter Display

The "PPC Smart Meter Display — V2" design, built as a Next.js app that renders
at exactly **480 × 320** — the landscape size of a 3.5 inch panel.

Ten screens, no on-screen chrome, navigated entirely by swipe. One layout,
two panels.

| Route | Panel | Device | Design space |
| --- | --- | --- | --- |
| `/` | Guition ESP32-S3, AXS15231B | 480 × 320, 3.5" | 480 × 320 @ 1× |
| `/large` | CrowdPanel ESP32 HMI, DIS08070H | 800 × 480, 7" | 533 × 320 @ 1.5× |

Both panels are **320 design-pixels tall** — the small one natively, the large
one at 1.5× — so a single vertical rhythm serves both and only the width
changes. The 7 inch panel is proportionally wider (5:3 against 3:2), and those
extra 53 design-pixels go to whichever column already flexes. The charts grow;
nothing is letterboxed; no screen is laid out twice.

Chart widths are derived, not hardcoded (`src/lib/panel.ts`). On the small
panel the formulas return 182, 422 and 218 — the exact figures the design was
drawn at — which is how the refactor left it pixel-identical.

Point each device at its own route. Nothing else differs between them.

## The screen map

Sideways, complexity rises left to right. Up goes one period back. Down from
Home reaches the raw meter output.

|              | Appliances | Home          | Medium         | High         | Compare          |
| ------------ | ---------- | ------------- | -------------- | ------------ | ---------------- |
| **n − 1** ↑  | —          | Home · August | Medium · July  | High · 2025  | Compare · August |
| **current**  | Appliances | **Home**      | Medium · August| High · 2026  | Compare          |
| **raw** ↓    | —          | P1 telegram   | —              | —            | —                |

Home is the entry screen. The gesture direction is the direction you travel:
swipe up and the panel moves up the map, which is what the design's own legend
describes — "swipe up: one period back", with n-1 drawn above Home, and "swipe
down from Home" for the telegram drawn below it. The deck therefore slides
against your finger, like panning a camera rather than dragging paper. If
testing on the real panel prefers the other convention, flip `GESTURE` to `-1`
in `src/components/Deck.tsx`.

Arrow keys do the same thing, which is handy during development.

## Running it

```bash
npm install
npm run dev     # http://localhost:3000, reachable on the LAN at 0.0.0.0:3000
npm run build && npm start
```

The stage scales the 480 × 320 canvas to fit whatever viewport it lands in, so
the design can be checked at size in a desktop browser. On the panel the scale
is exactly 1 and nothing is resampled.

## A PDF of every screen

```bash
npm run dev          # in one terminal
npm run pdf          # in another
```

Writes `docs/ppc-smart-meter-display-screens.pdf`, eleven A4 landscape pages:

- **The screen map** — every screen as a thumbnail in its place, with a line
  to each screen you can reach from it. It answers "what sits next to what"
  without anyone having to hold the grid in their head.
- **Then each screen** on its own page at exactly twice its real size, with a
  note on what it is for and where it sits.

`/print` renders it, so both the map and the pages come from the same
components as the panel and cannot fall out of date. The thumbnails are the
real screens at 0.35, not screenshots.

It drives the Chrome that is already installed rather than pulling in a second
browser; set `CHROME` if it lives somewhere unusual.

Two things that page has to undo, both worth knowing if you edit it. The panel
styles fix the root to one viewport and paint a dark surround, which on paper
clips the job to a single page — `globals.css` reverses that under
`@media print`. And Chrome drops background colours when printing unless
`print-color-adjust: exact` says otherwise, which would take the telegram's
dark panel, the amber badges and every gradient with it.

## Where the data comes from

Every screen reads one `MeterSnapshot` (`src/lib/types.ts`). The server renders
the first paint from it and the client re-polls `/api/meter` every ten seconds,
keeping the last good reading if a poll fails.

To go live, point `readMeter()` in `src/app/api/meter/route.ts` at the real
feed. Nothing else changes.

The sample reading in `src/lib/data.ts` is the one the design was drawn
against. Its series were recovered from the artifact's chart geometry and
check out against the totals printed on the screens:

| Series          | Total      | Average       |
| --------------- | ---------- | ------------- |
| August, daily   | 650 kWh    | 21,0 kWh/day  |
| July, daily     | 640 kWh    | 20,6 kWh/day  |
| 2026, monthly   | 7.034 kWh  | 586 kWh/month |
| 2025, monthly   | 7.410 kWh  | 618 kWh/month |

## Running this on the Gugxiom ESP32-S3 board

Worth being direct about one thing: **the ESP32-S3 cannot render this page on
its own panel.** That board drives its 320 × 480 TFT from firmware — ESP-IDF or
Arduino, usually through LVGL — and has no browser or HTML engine. A Next.js
app needs something that can run one.

Three ways to land this, in the order I would try them:

1. **ESP32-S3 as the meter, a browser as the display.** The board reads the P1
   port and serves the reading as JSON over wifi; this app polls it and draws
   the screens. The display is then any browser in kiosk mode — a Raspberry Pi
   with a 3.5 inch screen, a wall-mounted tablet, an old phone. This is what
   the repo is built for, and `readMeter()` is the single place to wire it up.

2. **Browser on the same host, board as a USB sensor.** Same as above, with the
   board on a serial link instead of wifi. `readMeter()` reads the serial
   stream rather than fetching.

3. **The design on that exact panel.** This has to be firmware, not a web page:
   rebuild the screens in LVGL. The port is mechanical rather than hard — the
   gauge is an `lv_arc`, the daily chart an `lv_chart` bar series, the year an
   `lv_chart` line series — and everything you need is already isolated:
   `src/lib/tokens.ts` holds every colour and size, `src/lib/data.ts` the
   series. Treat this app as the reference render and the spec.

If the goal is "the screens on that board's own display", option 3 is the only
honest answer. If the goal is "the screens on a 3.5 inch screen, with the
board reading the meter", options 1 and 2 get there with this codebase as-is.

## Panel notes

- `viewport` is pinned to 480 × 320, scale locked, so a stray pinch cannot
  move the layout.
- Scrolling, selection, tap highlight and overscroll bounce are all off. The
  surface behaves like an instrument face, not a web page.
- `prefers-reduced-motion` drops the slide transition.
- Fonts are self-hosted at build time via `next/font`, so a panel with no
  internet still paints correctly.
- **Ping LCG** is the brand face and is first in the font stack, but it is not
  on Google Fonts. Source Sans 3 carries the design until the licensed file is
  dropped into `src/app/fonts` and loaded with `next/font/local`.
- **No `var()` in a font-family, and no `clip-path`.** Both are fine in a
  desktop browser and both were observed failing on the panel. A custom
  property that fails to substitute invalidates the whole declaration, so the
  browser falls back to its own default rather than to the next name in the
  list — and that default can be a bitmap monospace face. `clip-path` being
  ignored gave every bar a rounded foot hanging below the zero line. The
  stacks in `src/lib/tokens.ts` now name real families, and the bars are drawn
  as top-rounded paths (`src/components/ui/shapes.ts`).
- **If the panel still shows a fallback face**, the webfont itself is not
  loading: check that `/_next/static/media/*.woff2` is reachable from the
  device and that its browser supports woff2. The long fallback chain means a
  miss degrades to a real sans face rather than to a bitmap one.

## Layout

```
src/
  app/
    layout.tsx            fonts, viewport lock, global reset
    page.tsx              server-renders the first snapshot
    globals.css           the "this is a panel, not a page" reset
    api/meter/route.ts    the only data source
  components/
    Panel.tsx             wires the snapshot into the screen map
    Stage.tsx             holds 480 x 320 and scales it to the viewport
    Deck.tsx              the 2D swipe navigator
    screens/              one file per screen in the map
    ui/                   Gauge, FillBar, DailyBars, YearLine, CompareBars
  lib/
    tokens.ts             every colour, size and font in the design
    data.ts               the sample reading
    types.ts              the MeterSnapshot contract
    format.ts             European number formatting (1.812 kWh, 0,84 kW)
    useMeter.ts           polling, with last-good-reading fallback
```

## Where this differs from the artifact

Asked-for changes:

- **Home carries the PPC mark and reads "My Energy Coach".** The mark is the
  real ΔΕΗ asset, served from `public/ppc-logo.png` and also used as the
  favicon (`src/app/icon.png`). It is a 675px PNG rendered at 44px, so it
  stays crisp when the stage scales up on a desktop. If an SVG of the mark
  turns up, point `src` in `src/components/ui/BrandMark.tsx` at it — nothing
  else has to move.
- **A closed month has no dial.** A needle pointing at a live reading means
  nothing once the month is over, so the past-month Home shows what the month
  averaged instead: mean draw in kW, and mean consumption per day in kWh.
- **Both charts have a y axis now**, starting at zero, ending on a round
  number, with the gridlines drawn across the fill rather than under it. The
  daily chart picks its own ceiling (`src/components/ui/scale.ts`), so July
  and August both land on 0–30 and can be compared by flicking between them.
- **"higher" on Compare is 46px**, near enough the percentage that the two
  read as one phrase.
- **The P1 telegram shows nine lines, not the whole frame** — what the meter
  has counted, which way it is flowing, and the voltage. The equipment id,
  DSMR version and message blocks are dropped. Each line carries a short
  gloss; delete the second `<span>` in `Telegram.tsx` if you want it bare.

Carried over from the first pass, both from making the screens data-driven
rather than hand-placed:

- **The gauge arc and needle are computed** from `value / max`. The artifact's
  closed-August gauge was nudged a degree or two off that geometry by hand.
- **The peak label on the year chart moves** when the peak lands near the top
  of the scale, where there is no room above the dot for it — as it does on
  2025, whose January reading is the year's highest. It shifts alongside the
  dot with a halo behind it instead of being clipped.
