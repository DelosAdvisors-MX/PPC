import { COLOR, MONO_STACK } from "@/lib/tokens";
import { Screen } from "@/components/ui/Screen";
import type { TelegramLine } from "@/lib/types";

interface Props {
  /** Only the lines worth reading — the filtering happens in the data. */
  lines: TelegramLine[];
  /** Code prefix to pick out in mint — normally the instantaneous draw. */
  highlight: string;
}

/** The DSMR 5.0 frame straight off the meter. Swipe down from Home. */
export function Telegram({ lines, highlight }: Props) {
  return (
    <Screen title="Your smart meter P1 telegram" style={{ flexDirection: "column", gap: 10 }}>
      <div style={{ fontSize: 22, fontWeight: 600, flexShrink: 0 }}>
        Your smart meter P1 telegram
      </div>

      <div
        style={{
          flexGrow: 1,
          boxSizing: "border-box",
          background: COLOR.termBg,
          borderRadius: 10,
          padding: "10px 12px",
          display: "flex",
          flexDirection: "column",
          gap: 7,
          overflow: "hidden",
        }}
      >
        <div
          style={{
            display: "flex",
            alignItems: "center",
            justifyContent: "space-between",
            flexShrink: 0,
          }}
        >
          <span style={{ display: "flex", alignItems: "center", gap: 7 }}>
            <span
              style={{
                width: 7,
                height: 7,
                borderRadius: 4,
                background: COLOR.termLive,
              }}
            />
            <span
              style={{
                fontFamily: MONO_STACK,
                fontSize: 12,
                fontWeight: 500,
                color: COLOR.termLabel,
              }}
            >
              DSMR 5.0 Telegram
            </span>
          </span>
          <span style={{ fontFamily: MONO_STACK, fontSize: 12, color: COLOR.termDim }}>
            raw output
          </span>
        </div>

        <div
          style={{
            fontSize: 11.5,
            lineHeight: 1.72,
            color: COLOR.termText,
            overflow: "hidden",
          }}
        >
          {lines.map((line) => {
            const lit = line.code.startsWith(highlight);
            return (
              <div
                key={line.code}
                style={{
                  display: "flex",
                  alignItems: "baseline",
                  justifyContent: "space-between",
                  gap: 12,
                }}
              >
                <span
                  style={{
                    fontFamily: MONO_STACK,
                    whiteSpace: "pre",
                    color: lit ? COLOR.termHighlight : COLOR.termText,
                  }}
                >
                  {line.code}
                </span>
                <span
                  style={{
                    fontSize: 11,
                    whiteSpace: "nowrap",
                    color: lit ? COLOR.termHighlight : COLOR.termDim,
                  }}
                >
                  {line.gloss}
                </span>
              </div>
            );
          })}
        </div>
      </div>
    </Screen>
  );
}
