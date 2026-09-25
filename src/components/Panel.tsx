"use client";

import type { MeterSnapshot } from "@/lib/types";
import { useMeter } from "@/lib/useMeter";
import { Stage } from "./Stage";
import { Deck, type Cell } from "./Deck";
import { Home } from "./screens/Home";
import { Appliances } from "./screens/Appliances";
import { Medium } from "./screens/Medium";
import { High } from "./screens/High";
import { Compare } from "./screens/Compare";
import { Telegram } from "./screens/Telegram";

/** Column order is the design's: complexity rises left to right. */
const COL = { appliances: 0, home: 1, medium: 2, high: 3, compare: 4 } as const;
/** Row order is the design's: previous period above, raw output below. */
const ROW = { previous: -1, current: 0, raw: 1 } as const;

export function Panel({ snapshot }: { snapshot: MeterSnapshot }) {
  const meter = useMeter(snapshot);

  const cells: Cell[] = [
    {
      key: "appliances",
      col: COL.appliances,
      row: ROW.current,
      node: <Appliances {...meter.appliances} />,
    },
    { key: "home", col: COL.home, row: ROW.current, node: <Home data={meter.home} /> },
    { key: "home-prev", col: COL.home, row: ROW.previous, node: <Home data={meter.homePrev} /> },
    {
      key: "telegram",
      col: COL.home,
      row: ROW.raw,
      node: <Telegram lines={meter.telegram.lines} highlight={meter.telegram.highlight} />,
    },
    { key: "medium", col: COL.medium, row: ROW.current, node: <Medium data={meter.medium} /> },
    {
      key: "medium-prev",
      col: COL.medium,
      row: ROW.previous,
      node: <Medium data={meter.mediumPrev} />,
    },
    { key: "high", col: COL.high, row: ROW.current, node: <High data={meter.high} /> },
    { key: "high-prev", col: COL.high, row: ROW.previous, node: <High data={meter.highPrev} /> },
    { key: "compare", col: COL.compare, row: ROW.current, node: <Compare data={meter.compare} /> },
    {
      key: "compare-prev",
      col: COL.compare,
      row: ROW.previous,
      node: <Compare data={meter.comparePrev} />,
    },
  ];

  return (
    <Stage>
      <Deck cells={cells} start={{ col: COL.home, row: ROW.current }} />
    </Stage>
  );
}
