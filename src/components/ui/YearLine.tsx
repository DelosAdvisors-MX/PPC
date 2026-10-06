import { COLOR, SVG_FONT } from "@/lib/tokens";
import { MONTH_LABELS } from "@/lib/data";
import { thousands } from "@/lib/format";

const H = 174;
const BASE = 150;
const PLOT_LEFT = 40;
/** The line stops short of the right edge so the last dot is not clipped. */
const PLOT_INSET = 12;
const AXIS_X = 34;

/**
 * The y axis is fixed, not fitted: the anchor always sits at the same height,
 * so 2025 and 2026 can be compared by flicking between them without the chart
 * rescaling underneath. Anything past the ceiling is clamped.
 *
 * Anchored at 300 kWh for a two-person home, whose months run 195 to 330.
 * Raise it if the data outgrows it; the ticks follow.
 */
const AXIS_ANCHOR = 300;
const PX_PER_KWH = (BASE - 33.4) / AXIS_ANCHOR;
const CEILING = BASE / PX_PER_KWH;
/** The axis reads from zero up, so the line's height means something. */
const TICKS = [0, AXIS_ANCHOR / 3, (AXIS_ANCHOR * 2) / 3, AXIS_ANCHOR];

/** The axis is always a whole year, however many months have happened. */
const MONTHS_IN_YEAR = 12;
/** Every month is named. At twelve labels they want the smaller size. */
const LABELLED_MONTHS = Array.from({ length: MONTHS_IN_YEAR }, (_, i) => i);
const MONTH_LABEL_SIZE = 11;

interface Props {
  /** Drawing width in design pixels; the chart fills whatever it is given. */
  width: number;
  /** Up to twelve kWh figures, January first. A year in progress sends fewer. */
  values: number[];
  label: string;
}

export function YearLine({ width: W, values, label }: Props) {
  const PLOT_RIGHT = W - PLOT_INSET;
  // Spaced across the whole year rather than across the data, so a year in
  // progress leaves the rest of the axis empty instead of stretching to fill
  // it — the line stops where the months do.
  const step = (PLOT_RIGHT - PLOT_LEFT) / (MONTHS_IN_YEAR - 1);
  const x = (i: number) => PLOT_LEFT + i * step;
  const y = (v: number) => BASE - Math.min(v, CEILING) * PX_PER_KWH;

  const peak = Math.max(...values);
  const peakIndex = values.indexOf(peak);
  const low = Math.min(...values);
  const total = values.reduce((sum, v) => sum + v, 0);

  // The peak figure normally sits above its dot. A year that peaks near the
  // top of the scale leaves no room, so the label moves alongside the dot
  // instead — outward, so it never runs off either end of the plot.
  const peakX = x(peakIndex);
  const peakY = y(peak);
  const peakLabel =
    peakY - 10.7 >= 10
      ? { x: peakX, y: peakY - 10.7, anchor: "middle" as const }
      : peakIndex > values.length / 2
        ? { x: peakX - 10, y: peakY - 2, anchor: "end" as const }
        : { x: peakX + 10, y: peakY - 2, anchor: "start" as const };

  const points = values.map((v, i) => `${x(i).toFixed(1)},${y(v).toFixed(1)}`);
  const line = `M${points.join(" L")}`;
  const lastMonth = values.length - 1;
  const area = `${line} L${x(lastMonth).toFixed(1)},${BASE} L${PLOT_LEFT},${BASE} Z`;

  return (
    <svg
      width={W}
      height={H}
      viewBox={`0 0 ${W} ${H}`}
      role="img"
      aria-label={`${label}, January to December: lowest ${thousands(low)} kWh, highest ${thousands(peak)} kWh in ${MONTH_LABELS[peakIndex]}, total ${thousands(total)} kWh`}
    >
      {/* The fill goes down first so the gridlines can be read across it. */}
      <path d={area} fill={COLOR.blueWash} />

      {TICKS.map((v) => (
        <line
          key={v}
          x1={AXIS_X}
          y1={y(v).toFixed(1)}
          x2={W}
          y2={y(v).toFixed(1)}
          stroke={v === 0 ? COLOR.axis : COLOR.gridLight}
          strokeWidth={1}
        />
      ))}

      {TICKS.map((v) => (
        <text
          key={v}
          x={AXIS_X - 6}
          y={(y(v) + 3.6).toFixed(1)}
          textAnchor="end"
          fontFamily={SVG_FONT}
          fontSize={11.5}
          fontWeight={600}
          fill={COLOR.muted}
        >
          {v}
        </text>
      ))}

      <path
        d={line}
        fill="none"
        stroke={COLOR.blue}
        strokeWidth={4}
        strokeLinecap="round"
        strokeLinejoin="round"
      />

      {values.map((value, i) => (
        <circle key={i} cx={x(i).toFixed(1)} cy={y(value).toFixed(1)} r={4} fill={COLOR.blue} />
      ))}

      <circle
        cx={x(peakIndex).toFixed(1)}
        cy={y(peak).toFixed(1)}
        r={6}
        fill={COLOR.magenta}
        stroke={COLOR.surface}
        strokeWidth={2}
      />
      <text
        x={peakLabel.x.toFixed(1)}
        y={peakLabel.y.toFixed(1)}
        textAnchor={peakLabel.anchor}
        fontFamily={SVG_FONT}
        fontSize={12.5}
        fontWeight={700}
        fill={COLOR.magenta}
        // Beside the dot the label can land on the line; the halo keeps it
        // readable without moving it off the point it belongs to.
        stroke={COLOR.surface}
        strokeWidth={3}
        paintOrder="stroke"
      >
        {thousands(peak)}
      </text>

      {/* Months still to come keep their label but fade, so the gap reads as
          "not yet" rather than as a chart that stops early. */}
      {LABELLED_MONTHS.map((i) => (
        <text
          key={i}
          x={x(i).toFixed(1)}
          y={168}
          textAnchor="middle"
          fontFamily={SVG_FONT}
          fontSize={MONTH_LABEL_SIZE}
          fontWeight={700}
          fill={i < values.length ? COLOR.muted : COLOR.axis}
        >
          {MONTH_LABELS[i]}
        </text>
      ))}
    </svg>
  );
}
