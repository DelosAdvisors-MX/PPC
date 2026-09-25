"use client";

import { useEffect, useState, type ReactNode } from "react";
import { PANEL } from "@/lib/tokens";

/**
 * Holds the panel at exactly 480 x 320 CSS pixels and scales it to whatever
 * viewport it lands in. On the 3.5 inch screen the scale is 1 and nothing
 * moves; on a desktop browser it grows so the design can be checked at size.
 */
export function Stage({ children }: { children: ReactNode }) {
  const [scale, setScale] = useState(1);

  useEffect(() => {
    const fit = () => {
      const next = Math.min(
        window.innerWidth / PANEL.width,
        window.innerHeight / PANEL.height,
      );
      // Never shrink below a readable size; the panel itself is always >= 1.
      setScale(next > 0 ? next : 1);
    };
    fit();
    window.addEventListener("resize", fit);
    window.addEventListener("orientationchange", fit);
    return () => {
      window.removeEventListener("resize", fit);
      window.removeEventListener("orientationchange", fit);
    };
  }, []);

  return (
    <div className="stage">
      <div
        className="stage-panel"
        style={{
          width: PANEL.width,
          height: PANEL.height,
          transform: `scale(${scale})`,
        }}
      >
        {children}
      </div>
    </div>
  );
}
