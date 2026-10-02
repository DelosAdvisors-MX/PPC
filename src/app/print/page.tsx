import type { ReactNode } from "react";
import { SAMPLE } from "@/lib/data";
import { Home } from "@/components/screens/Home";
import { Appliances } from "@/components/screens/Appliances";
import { Medium } from "@/components/screens/Medium";
import { High } from "@/components/screens/High";
import { Compare } from "@/components/screens/Compare";
import { Telegram } from "@/components/screens/Telegram";
import { PANELS, type PanelSpec } from "@/lib/panel";
import { NavigationMap, type MapCell } from "./NavigationMap";
import "./print.css";

export const dynamic = "force-dynamic";

export const metadata = {
  title: "PPC Smart Meter Display — all screens",
};

interface Sheet {
  title: string;
  /** Where the screen sits in the navigation map. */
  position: string;
  note: string;
  screen: ReactNode;
}

/** Reading order: down each column, left to right, as the map is drawn. */
const SHEETS: Sheet[] = [
  {
    title: "Appliances",
    position: "column 1 · current",
    note: "Consumption by machine, so far this year. Bars are relative to the heaviest appliance; the figure on the right is that appliance's share of the whole year, which is why the two disagree. No previous-period view.",
    screen: <Appliances {...SAMPLE.appliances} />,
  },
  {
    title: "Home",
    position: "column 2 · current · entry screen",
    note: "Instantaneous draw on the left, progress against the month's estimate on the right. This is where the panel starts.",
    screen: <Home data={SAMPLE.home} />,
  },
  {
    title: "Home · September",
    position: "column 2 · previous period",
    note: "A closed month has no live reading, so the dial is replaced by what the month averaged. Past 100% the bar stops at the ceiling and the overshoot is told in the caption.",
    screen: <Home data={SAMPLE.homePrev} />,
  },
  {
    title: "P1 telegram",
    position: "column 2 · raw",
    note: "The lines of the DSMR 5.0 frame worth reading: what the meter has counted, which way it is flowing, and the voltage. Reached by swiping down from Home.",
    screen: <Telegram lines={SAMPLE.telegram.lines} highlight={SAMPLE.telegram.highlight} />,
  },
  {
    title: "Medium · October",
    position: "column 3 · current",
    note: "One month, day by day. October is five days old, and the bars are spaced across all thirty-one so the month keeps its true width. The y axis ends on a round number, so two months share a scale and can be compared by flicking between them.",
    screen: <Medium data={SAMPLE.medium} />,
  },
  {
    title: "Medium · September",
    position: "column 3 · previous period",
    note: "Same screen, one period back.",
    screen: <Medium data={SAMPLE.mediumPrev} />,
  },
  {
    title: "High · 2026",
    position: "column 4 · current",
    note: "The year so far, January first. The line stops at the last closed month and leaves the rest of the axis empty rather than drawing months that have not happened. The y axis is fixed rather than fitted, so 2025 and 2026 compare without rescaling.",
    screen: <High data={SAMPLE.high} />,
  },
  {
    title: "High · 2025",
    position: "column 4 · previous period",
    note: "A closed year, all twelve months. January is its peak, which leaves no room above the dot — the label moves alongside it rather than being clipped.",
    screen: <High data={SAMPLE.highPrev} />,
  },
  {
    title: "Compare",
    position: "column 5 · current",
    note: "This home against the average of the comparison group, month for month. The percentage is derived from the two figures, not stored.",
    screen: <Compare data={SAMPLE.compare} />,
  },
  {
    title: "Compare · September",
    position: "column 5 · previous period",
    note: "The same comparison for one closed month.",
    screen: <Compare data={SAMPLE.comparePrev} />,
  },
];

/**
 * Where each screen sits. Column order is the design's — complexity rises left
 * to right — and the previous-period row is drawn above the current one, which
 * is the direction the legend describes: swipe up goes one period back.
 */
const mapFor = (panel: PanelSpec): MapCell[] => [
  { column: 1, row: 0, label: "Home · September", screen: <Home data={SAMPLE.homePrev} panel={panel} /> },
  { column: 2, row: 0, label: "Medium · September", screen: <Medium data={SAMPLE.mediumPrev} panel={panel} /> },
  { column: 3, row: 0, label: "High · 2025", screen: <High data={SAMPLE.highPrev} panel={panel} /> },
  { column: 4, row: 0, label: "Compare · September", screen: <Compare data={SAMPLE.comparePrev} panel={panel} /> },

  { column: 0, row: 1, label: "Appliances", screen: <Appliances {...SAMPLE.appliances} panel={panel} /> },
  { column: 1, row: 1, label: "Home", screen: <Home data={SAMPLE.home} panel={panel} /> },
  { column: 2, row: 1, label: "Medium · October", screen: <Medium data={SAMPLE.medium} panel={panel} /> },
  { column: 3, row: 1, label: "High · 2026", screen: <High data={SAMPLE.high} panel={panel} /> },
  { column: 4, row: 1, label: "Compare", screen: <Compare data={SAMPLE.compare} panel={panel} /> },

  {
    column: 1,
    row: 2,
    label: "P1 telegram",
    screen: <Telegram lines={SAMPLE.telegram.lines} highlight={SAMPLE.telegram.highlight} panel={panel} />,
  },
];

function MapSheet({ panel, lead }: { panel: PanelSpec; lead: boolean }) {
  const asOf = new Date().toISOString().slice(0, 10);
  return (
    <section className="sheet">
      <div className="cover-head">
        <div>
          <div className="cover-title">
            {lead ? "PPC Smart Meter Display" : "The same screens, 7 inch"}
          </div>
          <div className="cover-sub">
            {panel.label} · {SHEETS.length} screens
            {lead ? "" : " · one layout, widened rather than letterboxed"}
          </div>
        </div>
        <div className="cover-date">as of {asOf}</div>
      </div>

      <NavigationMap cells={mapFor(panel)} panel={panel} />

      {lead ? (
        <div className="legend">
          <b>Sideways</b> moves along a row, and complexity rises left to right: Appliances is a
          list, Compare is a judgement. <b>Up</b> goes one period back — the same screen, last
          month or last year. <b>Down from Home</b> reaches the raw meter output. Every line above
          is travelled in both directions, and there is no other way through: the panel shows no
          chrome, so the gestures are the whole navigation. Appliances has no previous period and
          the telegram hangs under Home alone, so moving sideways off the n − 1 row drops you back
          to the current one wherever the next column has no previous screen.
        </div>
      ) : (
        <div className="legend">
          Both panels are 320 design-pixels tall — the 3.5 inch one natively, the 7 inch one at
          1.5× — so a single vertical rhythm serves both and only the width changes. The 7 inch
          panel is proportionally wider, and those extra 53 design-pixels go to whichever column
          already flexes: the charts grow, nothing is letterboxed, and no screen is laid out
          twice. The pages that follow are the 3.5 inch panel.
        </div>
      )}
    </section>
  );
}

export default function PrintPage() {
  return (
    <div className="print-root">
      <MapSheet panel={PANELS.small} lead />
      <MapSheet panel={PANELS.large} lead={false} />
      {SHEETS.map((sheet, index) => (
        <section className="sheet" key={sheet.title}>
          <div className="sheet-head">
            <span className="sheet-title">
              {index + 1}. {sheet.title}
            </span>
            <span className="sheet-meta">{sheet.position}</span>
          </div>
          <div className="screen-frame">
            <div className="screen-scale">{sheet.screen}</div>
          </div>
          <p className="sheet-note">{sheet.note}</p>
        </section>
      ))}
    </div>
  );
}
