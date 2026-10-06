import { COLOR, SVG_FONT } from "@/lib/tokens";
import { kwh, percentOf } from "@/lib/format";
import { topRoundedBar } from "./shapes";

const H = 206;
/** Left of this is the percentage scale. */
const PLOT_LEFT = 42;
const TOP = 12; // the 100% line
const BASE = 196; // the 0% line
const SPAN = BASE - TOP;
const RADIUS = 8;
/** The bar takes this share of the plot, centred. Wide: on a panel this small
    the bar is the reading, and the white either side of it says nothing. */
const BAR_SHARE = 0.74;

interface Props {
  /** Drawing width in design pixels; the chart fills whatever it is given. */
  width: number;
  usedKwh: number;
  estimateKwh: number;
  /** A closed month paints magenta and draws the 100% line over the bar. */
  closed: boolean;
}

/**
 * The vertical "how far into the monthly estimate are we" bar.
 *
 * Past 100% the bar stops at the ceiling and the overshoot is told in the
 * caption, so a month that ran over still reads as a full bar rather than as
 * one that broke its own axis.
 */
export function FillBar({ width: W, usedKwh, estimateKwh, closed }: Props) {
  const plotWidth = W - PLOT_LEFT;
  const barWidth = Math.round(plotWidth * BAR_SHARE);
  const barX = PLOT_LEFT + Math.round((plotWidth - barWidth) / 2);

  const ratio = estimateKwh > 0 ? usedKwh / estimateKwh : 0;
  const over = ratio > 1;
  const top = Math.max(TOP, BASE - Math.min(ratio, 1) * SPAN);
  const accent = closed ? COLOR.magenta : COLOR.blue;

  // Above the bar normally; once the bar reaches the ceiling there is no room
  // left, so the figure moves inside it and flips to white.
  const labelInside = top < 24;

  return (
    <svg
      width={W}
      height={H}
      viewBox={`0 0 ${W} ${H}`}
      role="img"
      aria-label={`${kwh(usedKwh)} of the ${kwh(estimateKwh)} estimate, ${percentOf(usedKwh, estimateKwh)} percent`}
      style={{ marginTop: 6 }}
    >
      {!over && (
        <line x1={PLOT_LEFT} y1={TOP} x2={W} y2={TOP} stroke={COLOR.gridLight} strokeWidth={1} />
      )}
      <line x1={PLOT_LEFT} y1={104} x2={W} y2={104} stroke={COLOR.gridLight} strokeWidth={1} />
      <line x1={PLOT_LEFT} y1={BASE} x2={W} y2={BASE} stroke={COLOR.axis} strokeWidth={1} />

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
          fontWeight={700}
          fill={COLOR.muted}
        >
          {tick.text}
        </text>
      ))}

      <path d={topRoundedBar(barX, top, barWidth, BASE - top, RADIUS)} fill={accent} />

      {over && (
        <line
          x1={PLOT_LEFT}
          y1={TOP}
          x2={W}
          y2={TOP}
          stroke={COLOR.ink}
          strokeWidth={1.5}
          strokeDasharray="4 3"
        />
      )}

      <text
        x={barX + barWidth / 2}
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
