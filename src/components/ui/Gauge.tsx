import { COLOR, SVG_FONT } from "@/lib/tokens";

const CX = 120;
const CY = 120;
const ARC_R = 96;
const NEEDLE_R = 84;

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

/**
 * The half-dial on the Home screens: a grey track, a blue arc up to the
 * reading, and a needle. 240 x 152 in panel pixels.
 */
export function Gauge({ value, max, label }: Props) {
  const fraction = max > 0 ? value / max : 0;
  const tip = dial(fraction, ARC_R);
  const needle = dial(fraction, NEEDLE_R);
  const left = dial(0, ARC_R);
  const right = dial(1, ARC_R);

  return (
    <svg
      width={240}
      height={152}
      viewBox="0 0 240 152"
      role="img"
      aria-label={`${label}: ${value.toFixed(2).replace(".", ",")} kilowatts of a ${max} kilowatt scale`}
      style={{ marginTop: 6 }}
    >
      <path
        d={`M${left.x},${left.y} A${ARC_R},${ARC_R} 0 0 1 ${right.x},${right.y}`}
        fill="none"
        stroke={COLOR.gaugeTrack}
        strokeWidth={20}
        strokeLinecap="round"
      />
      {fraction > 0.001 && (
        <path
          d={`M${left.x},${left.y} A${ARC_R},${ARC_R} 0 0 1 ${tip.x.toFixed(1)},${tip.y.toFixed(1)}`}
          fill="none"
          stroke={COLOR.blue}
          strokeWidth={20}
          strokeLinecap="round"
        />
      )}
      <line
        x1={CX}
        y1={CY}
        x2={needle.x.toFixed(1)}
        y2={needle.y.toFixed(1)}
        stroke={COLOR.ink}
        strokeWidth={7}
        strokeLinecap="round"
      />
      <circle cx={CX} cy={CY} r={10} fill={COLOR.ink} />
      <text
        x={20}
        y={144}
        fontFamily={SVG_FONT}
        fontSize={13}
        fontWeight={600}
        fill={COLOR.muted}
      >
        0
      </text>
      <text
        x={220}
        y={144}
        textAnchor="end"
        fontFamily={SVG_FONT}
        fontSize={13}
        fontWeight={600}
        fill={COLOR.muted}
      >
        {max} kW
      </text>
    </svg>
  );
}
