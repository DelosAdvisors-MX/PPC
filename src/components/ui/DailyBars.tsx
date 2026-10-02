import { COLOR, SVG_FONT } from "@/lib/tokens";
import { decimal } from "@/lib/format";
import { zeroBasedTicks } from "./scale";
import { topRoundedBar } from "./shapes";

const W = 422;
const H = 174;
const BASE = 150; // the zero line; bars grow up from here
const TOP = 14; // where the top tick sits
const AXIS_X = 34; // left edge of the plot, labels sit to its left
const RADIUS = 3;

interface Props {
  /** One kWh figure per day so far. */
  values: number[];
  /** Days the month holds. Bars are spaced across all of them. */
  daysInMonth: number;
  label: string;
}

/**
 * One bar per day, the peak day in magenta, the month's average as a dashed
 * rule. The y axis starts at zero and ends on a round number, so the bars can
 * be read as quantities rather than just compared with each other.
 */
export function DailyBars({ values, daysInMonth, label }: Props) {
  const days = values.length;
  const peak = Math.max(...values);
  const peakIndex = values.indexOf(peak);
  const low = Math.min(...values);
  const average = values.reduce((sum, v) => sum + v, 0) / days;

  const { max, ticks } = zeroBasedTicks(peak);
  const y = (v: number) => BASE - (v / max) * (BASE - TOP);

  // Spaced across the whole month, not across the data: five days into
  // October should look like five days into October, not like a five-day
  // month. Same reasoning as the year chart leaving its unlived months empty.
  const band = (W - AXIS_X) / Math.max(daysInMonth, days);
  const barW = Math.max(4, Math.min(11, band * 0.72));
  const barX = (i: number) => AXIS_X + i * band + (band - barW) / 2;
  const centre = (i: number) => AXIS_X + i * band + band / 2;

  const dayTicks = [1, 8, 15, 22, 29].filter((d) => d <= daysInMonth);

  return (
    <svg
      width={W}
      height={H}
      viewBox={`0 0 ${W} ${H}`}
      role="img"
      aria-label={`${label}: average ${decimal(average)} kWh a day, highest ${decimal(peak)} on the ${peakIndex + 1}, lowest ${decimal(low)}`}
    >
      {ticks.map((tick) => (
        <line
          key={tick}
          x1={AXIS_X}
          y1={y(tick).toFixed(1)}
          x2={W}
          y2={y(tick).toFixed(1)}
          stroke={tick === 0 ? COLOR.axis : COLOR.gridFaint}
          strokeWidth={1}
        />
      ))}
      {ticks.map((tick) => (
        <text
          key={tick}
          x={AXIS_X - 6}
          y={(y(tick) + 3.6).toFixed(1)}
          textAnchor="end"
          fontFamily={SVG_FONT}
          fontSize={11.5}
          fontWeight={700}
          fill={COLOR.muted}
        >
          {tick}
        </text>
      ))}

      {values.map((value, i) => (
        <path
          key={i}
          d={topRoundedBar(barX(i), y(value), barW, BASE - y(value), RADIUS)}
          fill={i === peakIndex ? COLOR.magenta : COLOR.blue}
        />
      ))}

      <line
        x1={AXIS_X}
        y1={y(average).toFixed(1)}
        x2={W}
        y2={y(average).toFixed(1)}
        stroke={COLOR.muted}
        strokeWidth={1.5}
        strokeDasharray="5 4"
      />

      <text
        x={centre(peakIndex).toFixed(1)}
        y={(y(peak) - 4).toFixed(1)}
        textAnchor="middle"
        fontFamily={SVG_FONT}
        fontSize={12.5}
        fontWeight={700}
        fill={COLOR.magenta}
        stroke={COLOR.surface}
        strokeWidth={3}
        paintOrder="stroke"
      >
        {decimal(peak)}
      </text>

      {dayTicks.map((day) => (
        <text
          key={day}
          x={centre(day - 1).toFixed(1)}
          y={168}
          textAnchor="middle"
          fontFamily={SVG_FONT}
          fontSize={12}
          fontWeight={700}
          fill={day <= days ? COLOR.muted : COLOR.axis}
        >
          {day}
        </text>
      ))}
    </svg>
  );
}
