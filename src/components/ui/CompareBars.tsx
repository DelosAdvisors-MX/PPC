import { COLOR, SVG_FONT } from "@/lib/tokens";
import { thousands } from "@/lib/format";
import { topRoundedBar } from "./shapes";

const H = 238;
const BASE = 190;
const TALLEST = 174;
const RADIUS = 7;
/** Bar width and the gap between them, as shares of the chart — 66 and 34 of
    218 on the small panel. */
const BAR_SHARE = 0.38;
const GAP_SHARE = 0.11;

interface Props {
  /** Drawing width in design pixels; the chart fills whatever it is given. */
  width: number;
  myKwh: number;
  theirKwh: number;
  unitLabel: string;
}

/** This home against the comparison group's average — two bars, one axis. */
export function CompareBars({ width: W, myKwh, theirKwh, unitLabel }: Props) {
  const barWidth = Math.round(W * BAR_SHARE);
  const pairWidth = barWidth * 2 + Math.round(W * GAP_SHARE);
  const mineX = Math.round((W - pairWidth) / 2);
  const theirsX = mineX + pairWidth - barWidth;
  const scale = TALLEST / Math.max(myKwh, theirKwh, 1);
  const top = (v: number) => BASE - v * scale;

  const bars = [
    { x: mineX, value: myKwh, fill: COLOR.magenta },
    { x: theirsX, value: theirKwh, fill: COLOR.neutralBar },
  ];

  return (
    <svg
      width={W}
      height={H}
      viewBox={`0 0 ${W} ${H}`}
      role="img"
      aria-label={`Your home ${thousands(myKwh)} ${unitLabel}; similar homes ${thousands(theirKwh)}`}
    >
      <line x1={0} y1={BASE} x2={W} y2={BASE} stroke={COLOR.axis} strokeWidth={1} />

      {bars.map((bar) => (
        <path
          key={bar.x}
          d={topRoundedBar(bar.x, top(bar.value), barWidth, BASE - top(bar.value), RADIUS)}
          fill={bar.fill}
        />
      ))}

      {bars.map((bar) => (
        <text
          key={bar.x}
          x={bar.x + barWidth / 2}
          y={(top(bar.value) - 5).toFixed(1)}
          textAnchor="middle"
          fontFamily={SVG_FONT}
          fontSize={17}
          fontWeight={700}
          fill={COLOR.ink}
        >
          {thousands(bar.value)}
        </text>
      ))}

      <text
        x={mineX + barWidth / 2}
        y={210}
        textAnchor="middle"
        fontFamily={SVG_FONT}
        fontSize={13}
        fontWeight={700}
        fill={COLOR.ink}
      >
        Your home
      </text>
      <text
        x={theirsX + barWidth / 2}
        y={210}
        textAnchor="middle"
        fontFamily={SVG_FONT}
        fontSize={13}
        fontWeight={600}
        fill={COLOR.muted}
      >
        Similar
      </text>
      <text
        x={theirsX + barWidth / 2}
        y={226}
        textAnchor="middle"
        fontFamily={SVG_FONT}
        fontSize={13}
        fontWeight={600}
        fill={COLOR.muted}
      >
        homes
      </text>
    </svg>
  );
}
