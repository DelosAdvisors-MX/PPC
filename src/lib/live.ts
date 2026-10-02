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
  const swell = Math.sin((2 * Math.PI * t) / 22);
  const flicker = Math.sin((2 * Math.PI * t) / 3.1);
  return Math.min(4.8, Math.max(0.06, 2.3 + 1.95 * swell + 0.22 * flicker));
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
  const importedBase = 3376.586;
  const imported = importedBase + (elapsedMs / 3_600_000) * 2.3;

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
