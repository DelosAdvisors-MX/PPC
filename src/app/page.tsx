import { SAMPLE } from "@/lib/data";
import { Panel } from "@/components/Panel";

/** The reading is pulled per request so the first paint is already current. */
export const dynamic = "force-dynamic";

export default function Page() {
  return <Panel snapshot={SAMPLE} />;
}
