import { COLOR, INSIGHT_GRADIENT } from "@/lib/tokens";

interface Props {
  /** "August Insight" / "2026 Insight" */
  title: string;
  /** The averaged figure in the amber badge. */
  stat: string;
  /** "Avg. daily" / "Avg. monthly" */
  statLabel: string;
}

/** The gradient title pill plus amber average badge, shared by Medium and High. */
export function InsightHeader({ title, stat, statLabel }: Props) {
  return (
    <div style={{ height: 54, flexShrink: 0, display: "flex", gap: 10 }}>
      <div
        style={{
          flexGrow: 1,
          boxSizing: "border-box",
          borderRadius: 12,
          padding: "0 16px",
          display: "flex",
          alignItems: "center",
          background: INSIGHT_GRADIENT,
        }}
      >
        <span
          style={{
            fontSize: 21,
            fontWeight: 700,
            color: COLOR.surface,
            letterSpacing: "0.01em",
          }}
        >
          {title}
        </span>
      </div>
      <div
        style={{
          width: 150,
          flexShrink: 0,
          boxSizing: "border-box",
          borderRadius: 12,
          padding: "0 14px",
          display: "flex",
          flexDirection: "column",
          justifyContent: "center",
          background: COLOR.amber,
        }}
      >
        <span style={{ fontSize: 22, fontWeight: 700, lineHeight: 1.1, color: COLOR.amberInk }}>
          {stat}
        </span>
        <span
          style={{
            fontSize: 12,
            fontWeight: 700,
            letterSpacing: "0.08em",
            textTransform: "uppercase",
            color: COLOR.amberInk,
          }}
        >
          {statLabel}
        </span>
      </div>
    </div>
  );
}
