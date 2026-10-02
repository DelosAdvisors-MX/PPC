"use client";

import { useEffect, useState, type ReactNode } from "react";
import type { PanelSpec } from "@/lib/panel";

/**
 * Holds the panel at its design size and scales it to the device.
 *
 * Two scales multiply here. The panel's own — 1 on the 3.5 inch screen, 1.5
 * on the 7 inch one — turns design pixels into device pixels. The fit scale
 * then shrinks or grows that to whatever viewport it lands in, which is 1 on
 * the real hardware and only matters when the design is being reviewed in a
 * desktop browser.
 */
export function Stage({ panel, children }: { panel: PanelSpec; children: ReactNode }) {
  const [fit, setFit] = useState(1);

  useEffect(() => {
    const measure = () => {
      const next = Math.min(
        window.innerWidth / panel.deviceWidth,
        window.innerHeight / panel.deviceHeight,
      );
      setFit(next > 0 ? next : 1);
    };
    measure();
    window.addEventListener("resize", measure);
    window.addEventListener("orientationchange", measure);
    return () => {
      window.removeEventListener("resize", measure);
      window.removeEventListener("orientationchange", measure);
    };
  }, [panel]);

  return (
    <div className="stage">
      <div
        className="stage-panel"
        style={{
          width: panel.width,
          height: panel.height,
          transform: `scale(${panel.scale * fit})`,
        }}
      >
        {children}
      </div>
    </div>
  );
}
