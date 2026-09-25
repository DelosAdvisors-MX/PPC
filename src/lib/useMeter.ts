"use client";

import { useEffect, useState } from "react";
import type { MeterSnapshot } from "./types";

/** How often the panel asks for a fresh reading. */
const POLL_MS = 10_000;

/**
 * Keeps the panel on the latest reading. It starts from the snapshot the
 * server rendered, so the first paint is immediate and the screen never
 * flashes empty if the meter is unreachable.
 */
export function useMeter(initial: MeterSnapshot): MeterSnapshot {
  const [snapshot, setSnapshot] = useState(initial);

  useEffect(() => {
    let cancelled = false;

    const pull = async () => {
      try {
        const res = await fetch("/api/meter", { cache: "no-store" });
        if (!res.ok) throw new Error(`meter responded ${res.status}`);
        const next = (await res.json()) as MeterSnapshot;
        if (!cancelled) setSnapshot(next);
      } catch (error) {
        // A missed poll is normal on a panel behind flaky wifi. Keep showing
        // the last good reading and say so in the console rather than
        // blanking the screen.
        console.warn("meter poll failed, keeping last reading", error);
      }
    };

    const timer = setInterval(pull, POLL_MS);
    return () => {
      cancelled = true;
      clearInterval(timer);
    };
  }, []);

  return snapshot;
}
