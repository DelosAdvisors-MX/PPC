import type { CSSProperties, ReactNode } from "react";
import { COLOR } from "@/lib/tokens";
import { DEFAULT_PANEL, type PanelSpec } from "@/lib/panel";

interface Props {
  children: ReactNode;
  /** Layout for the screen's own content box. */
  style?: CSSProperties;
  title: string;
  /** Which panel this is being drawn for. Defaults to the 3.5 inch one. */
  panel?: PanelSpec;
}

/** The canvas every screen is drawn on, in the panel's design pixels. */
export function Screen({ children, style, title, panel = DEFAULT_PANEL }: Props) {
  return (
    <section
      aria-label={title}
      style={{
        width: panel.width,
        height: panel.height,
        boxSizing: "border-box",
        overflow: "hidden",
        padding: panel.padding,
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
