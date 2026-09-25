/**
 * Design tokens lifted from the "PPC Smart Meter Display — V2" artifact.
 * Every screen is 480 x 320 with a 16px gutter, so the numbers here are
 * literal CSS pixels on the panel — do not scale them.
 */

export const PANEL = {
  width: 480,
  height: 320,
  padding: 16,
} as const;

export const COLOR = {
  /** Page background and the body of every card. */
  surface: "#FFFFFF",
  /** Primary text. */
  ink: "#101720",
  /** Secondary text, axis labels, captions. */
  muted: "#5A6470",
  /** Text inside the filter chips. */
  chipInk: "#3A434F",
  /** Chip / progress-track fill. */
  wash: "#EDF0F4",

  /** The accent: live draw, daily bars, the year line. */
  blue: "#2E6FD0",
  /** Fill under the year line. */
  blueWash: "#E3ECFA",
  /** Unfilled half of the gauge. */
  gaugeTrack: "#E7ECF2",

  /** Attention: peak day, over-estimate, "higher than similar homes". */
  magenta: "#D6006E",
  /** The comparison group's bar. */
  neutralBar: "#9AA3AD",
  /** Appliance bars. */
  orange: "#E85A14",

  /** The "Avg." badge. */
  amber: "#FFC93C",
  amberInk: "#3D2A00",

  /** Card borders and gridlines, light to heavy. */
  gridFaint: "#EDF0F4",
  gridLight: "#DFE4EB",
  axis: "#C6CEDA",
  border: "#DFE4EB",

  /** The P1 telegram terminal. */
  termBg: "#12161C",
  termText: "#C7D1DE",
  termLabel: "#9AA6B6",
  termDim: "#6B7787",
  termLive: "#12907D",
  termHighlight: "#4FD6B4",
} as const;

/** Header pill on the Medium / High screens. */
export const INSIGHT_GRADIENT =
  "linear-gradient(120deg, #15897E 0%, #0E4C86 100%)";

/**
 * "Ping LCG" is the brand face. It is not on Google Fonts, so Source Sans 3
 * carries the design until the licensed file is dropped into src/app/fonts
 * and wired up with next/font/local.
 */
export const FONT_STACK =
  "'Ping LCG', var(--font-sans), system-ui, -apple-system, sans-serif";
export const MONO_STACK = "var(--font-mono), ui-monospace, monospace";

/** Every inline SVG label uses this so it matches the DOM text around it. */
export const SVG_FONT = FONT_STACK;
