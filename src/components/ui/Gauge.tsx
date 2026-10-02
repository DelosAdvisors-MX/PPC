import { COLOR, SVG_FONT, loadColor } from "@/lib/tokens";

const WIDTH = 250;
const HEIGHT = 160;
const CX = 125;
const CY = 126;
const ARC_R = 102;
const ARC_WIDTH = 30;
const NEEDLE_R = 86;
const HUB_R = 12;

interface Props {
  /** Current draw in kW. */
  value: number;
  /** Full scale in kW — the right end of the arc. */
  max: number;
  label: string;
}

/** Point on the dial. 0 = hard left, `max` = hard right, sweeping over the top. */
function dial(fraction: number, radius: number) {
  const angle = Math.PI * Math.min(Math.max(fraction, 0), 1);
  return {
    x: CX - radius * Math.cos(angle),
    y: CY - radius * Math.sin(angle),
  };
}

function arcPath(from: number, to: number, radius: number) {
  const a = dial(from, radius);
  const b = dial(to, radius);
  return `M${a.x.toFixed(1)},${a.y.toFixed(1)} A${radius},${radius} 0 0 1 ${b.x.toFixed(1)},${b.y.toFixed(1)}`;
}

/**
 * The half dial on the Home screen.
 *
 * The arc takes the colour of the load band the reading falls in — green
 * under 2 kW, amber to 3,5, red above — so the dial is readable across a
 * room, before the number is. The track stays neutral: colouring it too
 * would leave the arc competing with its own background.
 */
export function Gauge({ value, max, label }: Props) {
  const fraction = max > 0 ? Math.min(Math.max(value / max, 0), 1) : 0;
  const needle = dial(fraction, NEEDLE_R);
  const accent = loadColor(fraction);

  return (
    <svg
      width={WIDTH}
      height={HEIGHT}
      viewBox={`0 0 ${WIDTH} ${HEIGHT}`}
      role="img"
      aria-label={`${label}: ${value.toFixed(2).replace(".", ",")} kilowatts of a ${max} kilowatt scale`}
      style={{ marginTop: 2 }}
    >
      <path
        d={arcPath(0, 1, ARC_R)}
        fill="none"
        stroke={COLOR.gaugeTrack}
        strokeWidth={ARC_WIDTH}
        strokeLinecap="round"
      />

      {fraction > 0.001 && (
        <path
          d={arcPath(0, fraction, ARC_R)}
          fill="none"
          stroke={accent}
          strokeWidth={ARC_WIDTH}
          strokeLinecap="round"
        />
      )}

      <line
        x1={CX}
        y1={CY}
        x2={needle.x.toFixed(1)}
        y2={needle.y.toFixed(1)}
        stroke={COLOR.ink}
        strokeWidth={8}
        strokeLinecap="round"
      />
      <circle cx={CX} cy={CY} r={HUB_R} fill={COLOR.ink} />

      <text
        x={10}
        y={154}
        fontFamily={SVG_FONT}
        fontSize={13}
        fontWeight={700}
        fill={COLOR.muted}
      >
        0
      </text>
      <text
        x={WIDTH - 10}
        y={154}
        textAnchor="end"
        fontFamily={SVG_FONT}
        fontSize={13}
        fontWeight={700}
        fill={COLOR.muted}
      >
        {max} kW
      </text>
    </svg>
  );
}
