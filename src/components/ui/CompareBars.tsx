import { COLOR, SVG_FONT } from "@/lib/tokens";
import { thousands } from "@/lib/format";
import { topRoundedBar } from "./shapes";

const W = 218;
const H = 238;
const BASE = 190;
const TALLEST = 174;
const BAR_W = 66;
const RADIUS = 7;
const MINE_X = 26;
const THEIRS_X = 126;

interface Props {
  myKwh: number;
  theirKwh: number;
  unitLabel: string;
}

/** This home against the comparison group's average — two bars, one axis. */
export function CompareBars({ myKwh, theirKwh, unitLabel }: Props) {
  const scale = TALLEST / Math.max(myKwh, theirKwh, 1);
  const top = (v: number) => BASE - v * scale;

  const bars = [
    { x: MINE_X, value: myKwh, fill: COLOR.magenta },
    { x: THEIRS_X, value: theirKwh, fill: COLOR.neutralBar },
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
          d={topRoundedBar(bar.x, top(bar.value), BAR_W, BASE - top(bar.value), RADIUS)}
          fill={bar.fill}
        />
      ))}

      {bars.map((bar) => (
        <text
          key={bar.x}
          x={bar.x + BAR_W / 2}
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
        x={MINE_X + BAR_W / 2}
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
        x={THEIRS_X + BAR_W / 2}
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
        x={THEIRS_X + BAR_W / 2}
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
