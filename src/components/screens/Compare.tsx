import { COLOR } from "@/lib/tokens";
import { percentHigher, thousands } from "@/lib/format";
import type { CompareData } from "@/lib/types";
import { DEFAULT_PANEL, LAYOUT, chartWidth, type PanelSpec } from "@/lib/panel";
import { Screen, CARD, EYEBROW } from "@/components/ui/Screen";
import { CompareBars } from "@/components/ui/CompareBars";

/** This home against similar homes. The last page to the right. */
export function Compare({ data, panel = DEFAULT_PANEL }: { data: CompareData; panel?: PanelSpec }) {
  const delta = percentHigher(data.myKwh, data.theirKwh);
  const higher = delta >= 0;
  const accent = higher ? COLOR.magenta : COLOR.blue;

  return (
    <Screen title="Compared with similar homes" panel={panel} style={{ gap: LAYOUT.COMPARE_GAP }}>
      <div
        style={{
          width: LAYOUT.COMPARE_COLUMN,
          flexShrink: 0,
          minWidth: 0,
          overflow: "hidden",
          display: "flex",
          flexDirection: "column",
        }}
      >
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
            fontWeight: 700,
            lineHeight: 1.3,
            color: COLOR.ink,
            marginTop: 8,
            // A longer comparison label must wrap inside the column rather
            // than push the chart off the panel.
            overflowWrap: "break-word",
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
                fontWeight: 700,
                color: COLOR.ink,
                whiteSpace: "nowrap",
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
            fontWeight: 700,
            color: COLOR.chipInk,
            marginTop: "auto",
          }}
        >
          {thousands(data.groupSize)} similar homes
        </div>
      </div>

      <div style={CARD}>
        <div style={{ ...EYEBROW, height: 18, flexShrink: 0 }}>{data.unitLabel}</div>
        <CompareBars width={chartWidth.compare(panel)} myKwh={data.myKwh} theirKwh={data.theirKwh} unitLabel={data.unitLabel} />
      </div>
    </Screen>
  );
}
