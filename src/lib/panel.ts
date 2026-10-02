/**
 * The two panels, in one coordinate system.
 *
 * Both are 320 design-pixels tall — the 3.5 inch panel natively, the 7 inch
 * one at 1.5x — so a single vertical rhythm serves both and only the width
 * changes. The 7 inch panel is proportionally wider (5:3 against 3:2), and
 * that extra 53 design-pixels goes to whichever column already flexes rather
 * than to a letterbox.
 */

const DESIGN_HEIGHT = 320;
const PADDING = 16;

export type PanelId = "small" | "large";

export interface PanelSpec {
  id: PanelId;
  label: string;
  /** Design-space width. Layout and type are authored in these units. */
  width: number;
  /** Always DESIGN_HEIGHT. */
  height: number;
  padding: number;
  /** Physical pixels the panel has. */
  deviceWidth: number;
  deviceHeight: number;
  /** Device pixels per design pixel. */
  scale: number;
}

function panel(id: PanelId, label: string, deviceWidth: number, deviceHeight: number): PanelSpec {
  const scale = deviceHeight / DESIGN_HEIGHT;
  return {
    id,
    label,
    width: deviceWidth / scale,
    height: DESIGN_HEIGHT,
    padding: PADDING,
    deviceWidth,
    deviceHeight,
    scale,
  };
}

export const PANELS: Record<PanelId, PanelSpec> = {
  /** Guition / Gugxiom ESP32-S3, AXS15231B QSPI. */
  small: panel("small", "3.5 inch · 480 × 320", 480, 320),
  /** CrowdPanel ESP32 HMI, DIS08070H. */
  large: panel("large", "7 inch · 800 × 480", 800, 480),
};

export const DEFAULT_PANEL = PANELS.small;

/** Fixed columns the layouts reserve, in design pixels. */
const HOME_COLUMN = 250;
const HOME_GAP = 16;
const COMPARE_COLUMN = 190;
const COMPARE_GAP = 14;
/** A bordered card's own padding plus its 1px border, both sides. */
const CARD_INSET = 26;

const content = (p: PanelSpec) => p.width - p.padding * 2;

/**
 * How wide each chart may draw. Derived rather than hardcoded so the charts
 * grow with the panel — on the small panel these return 182, 422 and 218,
 * which are the figures the design was drawn at.
 */
export const chartWidth = {
  homeFill: (p: PanelSpec) => content(p) - HOME_COLUMN - HOME_GAP,
  card: (p: PanelSpec) => content(p) - CARD_INSET,
  compare: (p: PanelSpec) => content(p) - COMPARE_COLUMN - COMPARE_GAP - CARD_INSET,
};

export const LAYOUT = { HOME_COLUMN, HOME_GAP, COMPARE_COLUMN, COMPARE_GAP } as const;
