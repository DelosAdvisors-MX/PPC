import { COLOR, SVG_FONT } from "@/lib/tokens";
import { kwh, percentOf } from "@/lib/format";

const PLOT_LEFT = 42;
const PLOT_RIGHT = 182;
const TOP = 12; // the 100% line
const BASE = 196; // the 0% line
const SPAN = BASE - TOP;
const BAR_X = 74;
const BAR_W = 80;
const RADIUS = 8;

interface Props {
  usedKwh: number;
  estimateKwh: number;
  /** A closed month paints magenta and draws the 100% line over the bar. */
  closed: boolean;
}

/**
 * The vertical "how far into the monthly estimate are we" bar, 182 x 206.
 *
 * The bar is drawn taller than it looks and clipped at the baseline, so the
 * top corners round and the bottom stays square against the axis. Past 100%
 * the bar stops at the ceiling and the overshoot is told in the caption.
 */
export function FillBar({ usedKwh, estimateKwh, closed }: Props) {
  const ratio = estimateKwh > 0 ? usedKwh / estimateKwh : 0;
  const over = ratio > 1;
  const top = Math.max(TOP, BASE - Math.min(ratio, 1) * SPAN);
  const accent = closed ? COLOR.magenta : COLOR.blue;

  // Above the bar normally; once the bar reaches the ceiling there is no room
  // left, so the figure moves inside it and flips to white.
  const labelInside = top < 24;
  const clipId = closed ? "fillClipClosed" : "fillClipLive";

  return (
    <svg
      width={182}
      height={206}
      viewBox="0 0 182 206"
      role="img"
      aria-label={`${kwh(usedKwh)} of the ${kwh(estimateKwh)} estimate, ${percentOf(usedKwh, estimateKwh)} percent`}
      style={{ marginTop: 6 }}
    >
      <defs>
        <clipPath id={clipId}>
          <rect x={PLOT_LEFT} y={0} width={PLOT_RIGHT - PLOT_LEFT} height={BASE} />
        </clipPath>
      </defs>

      {!over && (
        <line x1={PLOT_LEFT} y1={TOP} x2={PLOT_RIGHT} y2={TOP} stroke={COLOR.gridLight} strokeWidth={1} />
      )}
      <line x1={PLOT_LEFT} y1={104} x2={PLOT_RIGHT} y2={104} stroke={COLOR.gridLight} strokeWidth={1} />
      <line x1={PLOT_LEFT} y1={BASE} x2={PLOT_RIGHT} y2={BASE} stroke={COLOR.axis} strokeWidth={1} />

      {[
        { text: "100%", y: 16 },
        { text: "50%", y: 108 },
        { text: "0%", y: 200 },
      ].map((tick) => (
        <text
          key={tick.text}
          x={36}
          y={tick.y}
          textAnchor="end"
          fontFamily={SVG_FONT}
          fontSize={13}
          fontWeight={600}
          fill={COLOR.muted}
        >
          {tick.text}
        </text>
      ))}

      <g clipPath={`url(#${clipId})`}>
        <rect
          x={BAR_X}
          y={top.toFixed(1)}
          width={BAR_W}
          height={(BASE - top + RADIUS).toFixed(1)}
          rx={RADIUS}
          fill={accent}
        />
      </g>

      {over && (
        <line
          x1={PLOT_LEFT}
          y1={TOP}
          x2={PLOT_RIGHT}
          y2={TOP}
          stroke={COLOR.ink}
          strokeWidth={1.5}
          strokeDasharray="4 3"
        />
      )}

      <text
        x={BAR_X + BAR_W / 2}
        y={labelInside ? top + 23 : top - 8}
        textAnchor="middle"
        fontFamily={SVG_FONT}
        fontSize={16}
        fontWeight={700}
        fill={labelInside ? COLOR.surface : COLOR.ink}
      >
        {kwh(usedKwh)}
      </text>
    </svg>
  );
}
