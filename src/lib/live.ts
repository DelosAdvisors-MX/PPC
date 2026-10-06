import type { MeterSnapshot, TelegramLine } from "./types";

/**
 * A demo reading that sweeps the whole dial.
 *
 * Deterministic — no Math.random — so the server and the first client render
 * agree, and so the gauge reliably passes through all three load bands
 * instead of hovering in one. Two sines: a slow swell across the range and a
 * small flicker on top, the way a real house behaves when a heavy appliance
 * cycles under a baseline.
 */
export function demoKilowatts(elapsedMs: number): number {
  const t = elapsedMs / 1000;
  // Standing load: fridge, router, standby. A two-person home sits here most
  // of the day.
  const base = 0.3;
  // The slow drift of lights, a television, a laptop.
  const household = 0.85 * (0.5 + 0.5 * Math.sin((2 * Math.PI * t) / 19));
  // One heavy appliance cycling — a water heater or the oven. Narrow, because
  // that is how it behaves: mostly off, briefly dominant. Raised to a sixth
  // power so it ramps through amber on the way to red rather than stepping.
  const appliance = 2.7 * Math.pow(Math.max(0, Math.sin((2 * Math.PI * t) / 28)), 6);
  const flicker = 0.05 * Math.sin((2 * Math.PI * t) / 2.3);
  return Math.min(4.9, Math.max(0.08, base + household + appliance + flicker));
}

const pad = (value: number, width: number) => String(value).padStart(width, "0");

/** DSMR fixed-width field: six digits, a dot, three decimals. */
const fixed = (value: number) => value.toFixed(3).padStart(10, "0");

function timestamp(now: Date): string {
  return (
    `${pad(now.getFullYear() % 100, 2)}${pad(now.getMonth() + 1, 2)}${pad(now.getDate(), 2)}` +
    `${pad(now.getHours(), 2)}${pad(now.getMinutes(), 2)}${pad(now.getSeconds(), 2)}`
  );
}

/** Where the live values land in the frame. */
const READING_TIME = "0-0:1.0.0";
const IMPORTED_LOW = "1-0:1.8.1";
export const DRAWING_NOW = "1-0:1.7.0";

/**
 * Rewrites the three lines of the telegram that move: the reading time, the
 * instantaneous draw, and the counter it feeds. Everything else is left
 * alone, so the frame still reads as one the meter sent.
 */
function liveTelegramLines(
  lines: readonly TelegramLine[],
  kw: number,
  elapsedMs: number,
  now: Date,
): TelegramLine[] {
  // Matches the register the telegram starts from, creeping at the home's
  // own average draw.
  const importedBase = 9427.183;
  const imported = importedBase + (elapsedMs / 3_600_000) * 0.9;

  return lines.map((line) => {
    if (line.code.startsWith(READING_TIME)) {
      return { ...line, code: `${READING_TIME}(${timestamp(now)}W)` };
    }
    if (line.code.startsWith(DRAWING_NOW)) {
      return { ...line, code: `${DRAWING_NOW}(${fixed(kw)}*kW)` };
    }
    if (line.code.startsWith(IMPORTED_LOW)) {
      return { ...line, code: `${IMPORTED_LOW}(${fixed(imported)}*kWh)` };
    }
    return line;
  });
}

/**
 * Overlays the live draw onto a snapshot. Returns the snapshot untouched when
 * there is no live value yet, so the server render and the first client render
 * are identical.
 */
export function withLive(
  snapshot: MeterSnapshot,
  kw: number | null,
  elapsedMs: number,
  now: Date,
): MeterSnapshot {
  if (kw === null || snapshot.home.live === undefined) return snapshot;

  return {
    ...snapshot,
    home: {
      ...snapshot.home,
      live: { ...snapshot.home.live, kw },
    },
    telegram: {
      ...snapshot.telegram,
      lines: liveTelegramLines(snapshot.telegram.lines, kw, elapsedMs, now),
    },
  };
}
