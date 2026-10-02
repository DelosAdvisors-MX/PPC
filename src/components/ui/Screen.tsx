import type { CSSProperties, ReactNode } from "react";
import { COLOR, PANEL } from "@/lib/tokens";

interface Props {
  children: ReactNode;
  /** Layout for the screen's own content box. */
  style?: CSSProperties;
  title: string;
}

/** The 480 x 320 canvas every screen is drawn on. */
export function Screen({ children, style, title }: Props) {
  return (
    <section
      aria-label={title}
      style={{
        width: PANEL.width,
        height: PANEL.height,
        boxSizing: "border-box",
        overflow: "hidden",
        padding: PANEL.padding,
        background: COLOR.surface,
        display: "flex",
        ...style,
      }}
    >
      {children}
    </section>
  );
}

/** The bordered card the Medium / High / Compare charts sit in. */
export const CARD: CSSProperties = {
  flexGrow: 1,
  boxSizing: "border-box",
  border: `1px solid ${COLOR.border}`,
  borderRadius: 12,
  padding: 12,
  display: "flex",
  flexDirection: "column",
  gap: 6,
  overflow: "hidden",
};

/**
 * Small uppercase caption used under charts and beside units.
 *
 * Bold and near-black rather than grey: at 13px on a 3.5 inch panel held at
 * arm's length, grey-on-white is the first thing to disappear.
 */
export const EYEBROW: CSSProperties = {
  fontSize: 13,
  fontWeight: 700,
  letterSpacing: "0.08em",
  textTransform: "uppercase",
  color: COLOR.ink,
};
