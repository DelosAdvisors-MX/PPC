"use client";

import { useEffect, useState } from "react";
import { demoKilowatts } from "./live";

/** Fast enough that the needle sweeps rather than steps. */
const NEEDLE_MS = 100;

export interface LiveDemo {
  /** null until the client takes over, so the first paint matches the server. */
  kw: number | null;
  elapsedMs: number;
  now: Date;
}

/**
 * Drives the panel's live state.
 *
 * One signal feeds everything that moves: the needle, the reading under it,
 * and the telegram's three live fields. They cannot disagree because they are
 * the same number.
 */
export function useLiveDemo(enabled = true): LiveDemo {
  const [state, setState] = useState<LiveDemo>({ kw: null, elapsedMs: 0, now: new Date(0) });

  useEffect(() => {
    if (!enabled) return;
    const start = performance.now();

    const update = () => {
      const elapsedMs = performance.now() - start;
      setState({ kw: demoKilowatts(elapsedMs), elapsedMs, now: new Date() });
    };

    update();
    const timer = setInterval(update, NEEDLE_MS);
    return () => clearInterval(timer);
  }, [enabled]);

  return state;
}
