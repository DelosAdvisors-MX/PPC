import { COLOR } from "@/lib/tokens";
import { decimal, kwh } from "@/lib/format";
import type { HomeScreenData } from "@/lib/types";
import { DEFAULT_PANEL, LAYOUT, chartWidth, type PanelSpec } from "@/lib/panel";
import { Screen } from "@/components/ui/Screen";
import { BrandMark } from "@/components/ui/BrandMark";
import { Gauge } from "@/components/ui/Gauge";
import { FillBar } from "@/components/ui/FillBar";

const CAPTION = {
  fontSize: 13,
  fontWeight: 700,
  letterSpacing: "0.08em",
  textTransform: "uppercase",
  color: COLOR.ink,
} as const;

/** One figure with its caption, used down the closed month's left column. */
function Stat({ value, unit, caption, size }: { value: string; unit: string; caption: string; size: number ; panel?: PanelSpec }) {
  return (
    <div>
      <div style={{ display: "flex", alignItems: "baseline", gap: 8 }}>
        <span style={{ fontSize: size, fontWeight: 700, lineHeight: 1, color: COLOR.ink }}>
          {value}
        </span>
        <span style={{ fontSize: size * 0.42, fontWeight: 700, color: COLOR.ink }}>{unit}</span>
      </div>
      <div style={{ ...CAPTION, marginTop: 4 }}>{caption}</div>
    </div>
  );
}

/**
 * The entry screen: instantaneous draw on the left, progress against this
 * month's estimate on the right.
 *
 * A closed month runs the same layout but drops the dial — a needle pointing
 * at a live reading means nothing once the month is over — and shows what the
 * month averaged instead.
 */
export function Home({ data, panel = DEFAULT_PANEL }: { data: HomeScreenData; panel?: PanelSpec }) {
  const { live, averages, progress } = data;

  return (
    <Screen title={`Home — ${data.title}`} panel={panel} style={{ gap: LAYOUT.HOME_GAP }}>
      <div style={{ width: LAYOUT.HOME_COLUMN, flexShrink: 0, display: "flex", flexDirection: "column" }}>
        <div style={{ display: "flex", alignItems: "center", gap: 10, height: 44, flexShrink: 0 }}>
          <BrandMark />
          <span
            style={{
              fontSize: data.badge ? 20 : 22,
              fontWeight: 600,
              letterSpacing: "-0.01em",
            }}
          >
            {data.title}
          </span>
          {data.badge && (
            <span
              style={{
                fontSize: 13,
                fontWeight: 600,
                letterSpacing: "0.06em",
                textTransform: "uppercase",
                color: COLOR.muted,
              }}
            >
              {data.badge}
            </span>
          )}
        </div>

        {live && (
          <>
            <Gauge value={live.kw} max={live.scaleKw} label={live.caption} />
            <div style={{ display: "flex", alignItems: "baseline", gap: 8, marginTop: 2 }}>
              <span style={{ fontSize: 48, fontWeight: 700, lineHeight: 1, color: COLOR.ink }}>
                {decimal(live.kw, 2)}
              </span>
              <span style={{ fontSize: 20, fontWeight: 700, color: COLOR.muted }}>kW</span>
            </div>
            <div style={{ ...CAPTION, marginTop: 4 }}>{live.caption}</div>
          </>
        )}

        {averages && (
          <div
            style={{
              flexGrow: 1,
              display: "flex",
              flexDirection: "column",
              justifyContent: "center",
              gap: 22,
              paddingBottom: 8,
            }}
          >
            <Stat
              value={decimal(averages.kw, 2)}
              unit="kW"
              caption="Average draw"
              size={48}
            />
            <div style={{ height: 1, background: COLOR.gridLight }} />
            <Stat
              value={decimal(averages.kwhPerDay)}
              unit="kWh"
              caption="Average per day"
              size={34}
            />
          </div>
        )}
      </div>

      <div style={{ flexGrow: 1, display: "flex", flexDirection: "column" }}>
        <div
          style={{
            fontSize: 13,
            fontWeight: 700,
            lineHeight: 1.35,
            color: COLOR.ink,
            flexShrink: 0,
          }}
        >
          {kwh(progress.estimateKwh)} estimated
          <br />
          for {progress.month}
        </div>

        <FillBar
          width={chartWidth.homeFill(panel)}
          usedKwh={progress.usedKwh}
          estimateKwh={progress.estimateKwh}
          closed={progress.closed}
        />

        <div
          style={{
            ...CAPTION,
            color: progress.closed ? COLOR.magenta : COLOR.ink,
            marginTop: 4,
          }}
        >
          {progress.caption}
        </div>
      </div>
    </Screen>
  );
}
