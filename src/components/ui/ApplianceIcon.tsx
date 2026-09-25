import { COLOR } from "@/lib/tokens";
import type { Appliance } from "@/lib/types";

/** 24-grid line icons, one per appliance the disaggregation reports. */
const PATHS: Record<Appliance["id"], React.ReactNode> = {
  hotWater: <path d="M12 2.7s5.2 5.3 5.2 9a5.2 5.2 0 0 1-10.4 0c0-3.7 5.2-9 5.2-9z" />,
  tumbleDryer: (
    <>
      <rect x="3" y="3" width="18" height="18" rx="3" />
      <circle cx="12" cy="13" r="5" />
      <path d="M7 6.5h2" />
    </>
  ),
  oven: (
    <>
      <rect x="3" y="4" width="18" height="16" rx="2.5" />
      <path d="M3 9h18" />
      <circle cx="7" cy="6.5" r="0.9" />
      <rect x="7" y="12" width="10" height="5" rx="1.5" />
    </>
  ),
  hob: (
    <>
      <rect x="3" y="3" width="18" height="18" rx="3" />
      <circle cx="8.5" cy="8.5" r="1.6" />
      <circle cx="15.5" cy="8.5" r="1.6" />
      <circle cx="8.5" cy="15.5" r="1.6" />
      <circle cx="15.5" cy="15.5" r="1.6" />
    </>
  ),
  lighting: (
    <>
      <path d="M9 18h6" />
      <path d="M10 21h4" />
      <path d="M12 3a6 6 0 0 0-3.6 10.8c.5.4.8 1 .9 1.7h5.4c.1-.7.4-1.3.9-1.7A6 6 0 0 0 12 3z" />
    </>
  ),
};

export function ApplianceIcon({ id }: { id: Appliance["id"] }) {
  return (
    <svg
      width={26}
      height={26}
      viewBox="0 0 24 24"
      fill="none"
      stroke={COLOR.ink}
      strokeWidth={1.8}
      strokeLinecap="round"
      strokeLinejoin="round"
      aria-hidden="true"
      style={{ flexShrink: 0 }}
    >
      {PATHS[id]}
    </svg>
  );
}
