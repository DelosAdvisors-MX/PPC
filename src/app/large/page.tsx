import { SAMPLE } from "@/lib/data";
import { PANELS } from "@/lib/panel";
import { Panel } from "@/components/Panel";

/**
 * The 7 inch CrowdPanel. Same screens as `/`, 53 design-pixels wider, scaled
 * 1.5x to 800 x 480. Point that device at this route and the small one at the
 * root; nothing else differs between them.
 */
export const dynamic = "force-dynamic";

export default function LargePanelPage() {
  return <Panel snapshot={SAMPLE} panel={PANELS.large} />;
}
