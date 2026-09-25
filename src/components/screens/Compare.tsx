import { COLOR } from "@/lib/tokens";
import { percentHigher, thousands } from "@/lib/format";
import type { CompareData } from "@/lib/types";
import { Screen, CARD, EYEBROW } from "@/components/ui/Screen";
import { CompareBars } from "@/components/ui/CompareBars";

/** This home against similar homes. The last page to the right. */
export function Compare({ data }: { data: CompareData }) {
  const delta = percentHigher(data.myKwh, data.theirKwh);
  const higher = delta >= 0;
  const accent = higher ? COLOR.magenta : COLOR.blue;

  return (
    <Screen title="Compared with similar homes" style={{ gap: 14 }}>
      <div style={{ width: 190, flexShrink: 0, display: "flex", flexDirection: "column" }}>
        {data.eyebrow && <div style={EYEBROW}>{data.eyebrow}</div>}
        <div
          style={{
            fontSize: 72,
            fontWeight: 700,
            lineHeight: 0.94,
            color: accent,
            letterSpacing: "-0.03em",
            marginTop: data.eyebrow ? 2 : 0,
          }}
        >
          {Math.abs(delta)}%
        </div>
        <div
          style={{
            fontSize: 46,
            fontWeight: 700,
            lineHeight: 1.02,
            letterSpacing: "-0.02em",
            color: accent,
          }}
        >
          {higher ? "higher" : "lower"}
        </div>
        <div
          style={{
            fontSize: 13.5,
            fontWeight: 600,
            lineHeight: 1.3,
            color: COLOR.ink,
            marginTop: 8,
          }}
        >
          {data.subtitle}
        </div>

        <div
          style={{
            display: "flex",
            flexWrap: "wrap",
            gap: 4,
            marginTop: data.eyebrow ? 10 : 12,
          }}
        >
          {data.chips.map((chip) => (
            <span
              key={chip}
              style={{
                fontSize: 11,
                fontWeight: 600,
                color: COLOR.chipInk,
                background: COLOR.wash,
                borderRadius: 999,
                padding: "3px 8px",
              }}
            >
              {chip}
            </span>
          ))}
        </div>

        <div
          style={{
            fontSize: 12,
            fontWeight: 600,
            color: COLOR.muted,
            marginTop: "auto",
          }}
        >
          {thousands(data.groupSize)} similar homes
        </div>
      </div>

      <div style={CARD}>
        <div style={{ ...EYEBROW, height: 18, flexShrink: 0 }}>{data.unitLabel}</div>
        <CompareBars myKwh={data.myKwh} theirKwh={data.theirKwh} unitLabel={data.unitLabel} />
      </div>
    </Screen>
  );
}
