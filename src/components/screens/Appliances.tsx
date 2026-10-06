import { COLOR } from "@/lib/tokens";
import { decimal, kwh } from "@/lib/format";
import type { Appliance } from "@/lib/types";
import { DEFAULT_PANEL, type PanelSpec } from "@/lib/panel";
import { Screen } from "@/components/ui/Screen";
import { ApplianceIcon } from "@/components/ui/ApplianceIcon";

interface Props {
  headline: string;
  intro: string;
  items: Appliance[];
  panel?: PanelSpec;
}

/**
 * Consumption split by machine, so far this year. Bars are relative to the
 * heaviest appliance; the figure on the right is that appliance's share of
 * the whole year, which is why the bars and the percentages disagree.
 */
export function Appliances({ headline, intro, items, panel = DEFAULT_PANEL }: Props) {
  const heaviest = Math.max(...items.map((item) => item.kwh), 1);

  return (
    <Screen title={headline} panel={panel} style={{ flexDirection: "column" }}>
      <div style={{ fontSize: 22, fontWeight: 600, lineHeight: 1.15, flexShrink: 0 }}>
        {headline}
      </div>
      <div
        style={{
          fontSize: 13.5,
          fontWeight: 600,
          lineHeight: 1.35,
          color: COLOR.chipInk,
          marginTop: 4,
          flexShrink: 0,
        }}
      >
        {intro}
      </div>

      <div
        style={{
          flexGrow: 1,
          display: "flex",
          flexDirection: "column",
          justifyContent: "space-between",
          marginTop: 10,
        }}
      >
        {items.map((item) => (
          <div key={item.id} style={{ display: "flex", alignItems: "center", gap: 10 }}>
            <ApplianceIcon id={item.id} />
            <div style={{ flexGrow: 1, display: "flex", flexDirection: "column", gap: 5 }}>
              <span style={{ fontSize: 15, fontWeight: 600 }}>
                {item.name}: {kwh(item.kwh)}
              </span>
              <span style={{ display: "flex", alignItems: "center", gap: 10 }}>
                <span
                  style={{
                    flexGrow: 1,
                    height: 14,
                    borderRadius: 7,
                    background: COLOR.wash,
                  }}
                >
                  <span
                    style={{
                      display: "block",
                      width: `${((item.kwh / heaviest) * 100).toFixed(1)}%`,
                      height: 14,
                      borderRadius: 7,
                      background: COLOR.orange,
                    }}
                  />
                </span>
                <span
                  style={{
                    width: 46,
                    flexShrink: 0,
                    textAlign: "right",
                    fontSize: 12.5,
                    fontWeight: 700,
                    color: COLOR.ink,
                  }}
                >
                  {decimal(item.share)}%
                </span>
              </span>
            </div>
          </div>
        ))}
      </div>
    </Screen>
  );
}
