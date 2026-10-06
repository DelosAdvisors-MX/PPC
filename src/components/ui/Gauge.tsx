import { COLOR, SVG_FONT, loadColor } from "@/lib/tokens";
import { decimal } from "@/lib/format";

const WIDTH = 250;
const HEIGHT = 216;
const CX = 125;
const CY = 112;

/** Centreline of the arc, and how thick it is drawn. */
const RADIUS = 86;
const ARC_WIDTH = 26;

/**
 * A speed-test dial, not a half dial: it opens at the bottom and sweeps 270°,
 * which leaves room for graduations the whole way round and puts the reading
 * in the middle where the eye already is.
 *
 * Angles are screen degrees — 0 at 3 o'clock, growing clockwise, y down — so
 * 135° is the lower-left foot and 405° (45°) is the lower-right one.
 */
const START = 135;
const SWEEP = 270;

const NEEDLE_R = 66;
const HUB_R = 10;
/** Graduations sit outside the arc; their labels sit outside those. */
const TICK_INNER = RADIUS + ARC_WIDTH / 2 + 3;
const TICK_OUTER = TICK_INNER + 7;
const LABEL_R = TICK_OUTER + 13;

interface Props {
  /** Current draw in kW. */
  value: number;
  /** Full scale in kW — the far end of the sweep. */
  max: number;
  label: string;
}

const point = (degrees: number, radius: number) => {
  const radians = (degrees * Math.PI) / 180;
  return {
    x: CX + radius * Math.cos(radians),
    y: CY + radius * Math.sin(radians),
  };
};

const angleAt = (fraction: number) => START + SWEEP * Math.min(Math.max(fraction, 0), 1);

function arcPath(from: number, to: number, radius: number) {
  const a = point(from, radius);
  const b = point(to, radius);
  const largeArc = to - from > 180 ? 1 : 0;
  return `M${a.x.toFixed(1)},${a.y.toFixed(1)} A${radius},${radius} 0 ${largeArc} 1 ${b.x.toFixed(1)},${b.y.toFixed(1)}`;
}

export function Gauge({ value, max, label }: Props) {
  const fraction = max > 0 ? Math.min(Math.max(value / max, 0), 1) : 0;
  const accent = loadColor(fraction);
  const needle = point(angleAt(fraction), NEEDLE_R);

  // A graduation every half kilowatt, numbered on the whole ones.
  const steps = Math.round(max * 2);
  const ticks = Array.from({ length: steps + 1 }, (_, i) => {
    const kw = i / 2;
    return { kw, major: Number.isInteger(kw), angle: angleAt(kw / max) };
  });

  return (
    <svg
      width={WIDTH}
      height={HEIGHT}
      viewBox={`0 0 ${WIDTH} ${HEIGHT}`}
      role="img"
      aria-label={`${label}: ${decimal(value, 2)} of ${max} kilowatts`}
      style={{ marginTop: 2 }}
    >
      <path
        d={arcPath(START, START + SWEEP, RADIUS)}
        fill="none"
        stroke={COLOR.gaugeTrack}
        strokeWidth={ARC_WIDTH}
        strokeLinecap="round"
      />
      {fraction > 0.002 && (
        <path
          d={arcPath(START, angleAt(fraction), RADIUS)}
          fill="none"
          stroke={accent}
          strokeWidth={ARC_WIDTH}
          strokeLinecap="round"
        />
      )}

      {ticks.map((tick) => {
        const from = point(tick.angle, tick.major ? TICK_INNER : TICK_INNER + 3);
        const to = point(tick.angle, TICK_OUTER);
        return (
          <line
            key={tick.kw}
            x1={from.x.toFixed(1)}
            y1={from.y.toFixed(1)}
            x2={to.x.toFixed(1)}
            y2={to.y.toFixed(1)}
            stroke={tick.major ? COLOR.muted : COLOR.axis}
            strokeWidth={tick.major ? 2 : 1.5}
            strokeLinecap="round"
          />
        );
      })}

      {ticks
        .filter((tick) => tick.major)
        .map((tick) => {
          const at = point(tick.angle, LABEL_R);
          return (
            <text
              key={tick.kw}
              x={at.x.toFixed(1)}
              y={(at.y + 4).toFixed(1)}
              textAnchor="middle"
              fontFamily={SVG_FONT}
              fontSize={12}
              fontWeight={700}
              fill={COLOR.muted}
            >
              {tick.kw}
            </text>
          );
        })}

      <line
        x1={CX}
        y1={CY}
        x2={needle.x.toFixed(1)}
        y2={needle.y.toFixed(1)}
        stroke={COLOR.ink}
        strokeWidth={7}
        strokeLinecap="round"
      />
      <circle cx={CX} cy={CY} r={HUB_R} fill={COLOR.ink} />

      {/*
        The reading sits below the pivot, in the mouth of the dial. The sweep
        runs 135° to 405°, so the needle never points straight down — that
        space is the one place inside the dial it cannot reach.
      */}
      <text
        x={CX}
        y={CY + 46}
        textAnchor="middle"
        fontFamily={SVG_FONT}
        fontSize={44}
        fontWeight={700}
        fill={COLOR.ink}
      >
        {decimal(value, 2)}
      </text>
      <text
        x={CX}
        y={CY + 68}
        textAnchor="middle"
        fontFamily={SVG_FONT}
        fontSize={15}
        fontWeight={700}
        letterSpacing="0.08em"
        fill={COLOR.muted}
      >
        kW
      </text>
    </svg>
  );
}
