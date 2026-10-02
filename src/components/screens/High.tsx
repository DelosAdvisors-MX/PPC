import { COLOR } from "@/lib/tokens";
import { kwh, thousands } from "@/lib/format";
import type { YearInsight } from "@/lib/types";
import { Screen, CARD, EYEBROW } from "@/components/ui/Screen";
import { YearLine } from "@/components/ui/YearLine";
import { InsightHeader } from "./InsightHeader";

/** Twelve months, January first. */
export function High({ data }: { data: YearInsight }) {
  const total = data.monthly.reduce((sum, v) => sum + v, 0);
  const average = total / data.monthly.length;
  // A year in progress has no year total yet, only a running one.
  const complete = data.monthly.length >= 12;

  return (
    <Screen title={data.title} style={{ flexDirection: "column", gap: 10 }}>
      <InsightHeader title={data.title} stat={kwh(average)} statLabel="Avg. monthly" />

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
          <span style={EYEBROW}>kWh a month</span>
          <span style={{ fontSize: 13, fontWeight: 700, color: COLOR.ink }}>
            {complete ? "Total year" : "Total so far"} {thousands(total)} kWh
          </span>
        </div>

        <YearLine values={data.monthly} label={data.title} />
      </div>
    </Screen>
  );
}
