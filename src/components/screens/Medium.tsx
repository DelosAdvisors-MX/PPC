import { COLOR } from "@/lib/tokens";
import { kwhDecimal } from "@/lib/format";
import type { DailyInsight } from "@/lib/types";
import { Screen, CARD, EYEBROW } from "@/components/ui/Screen";
import { DailyBars } from "@/components/ui/DailyBars";
import { InsightHeader } from "./InsightHeader";

/** One month, day by day. */
export function Medium({ data }: { data: DailyInsight }) {
  const average = data.daily.reduce((sum, v) => sum + v, 0) / data.daily.length;

  return (
    <Screen title={data.title} style={{ flexDirection: "column", gap: 10 }}>
      <InsightHeader title={data.title} stat={kwhDecimal(average)} statLabel="Avg. daily" />

      <div style={CARD}>
        <div
          style={{
            display: "flex",
            alignItems: "center",
            justifyContent: "space-between",
            height: 18,
            flexShrink: 0,
          }}
        >
          <span style={EYEBROW}>kWh a day</span>
          <span style={{ display: "flex", alignItems: "center", gap: 14 }}>
            <span style={{ display: "flex", alignItems: "center", gap: 6 }}>
              <span style={{ width: 14, height: 0, borderTop: `2px dashed ${COLOR.muted}` }} />
              <span style={{ fontSize: 12, fontWeight: 600, color: COLOR.muted }}>Average</span>
            </span>
            <span style={{ display: "flex", alignItems: "center", gap: 6 }}>
              <span style={{ width: 9, height: 9, borderRadius: 2, background: COLOR.magenta }} />
              <span style={{ fontSize: 12, fontWeight: 600, color: COLOR.muted }}>Peak day</span>
            </span>
          </span>
        </div>

        <DailyBars values={data.daily} label={data.title} />
      </div>
    </Screen>
  );
}
