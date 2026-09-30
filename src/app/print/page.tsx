import type { ReactNode } from "react";
import { SAMPLE } from "@/lib/data";
import { Home } from "@/components/screens/Home";
import { Appliances } from "@/components/screens/Appliances";
import { Medium } from "@/components/screens/Medium";
import { High } from "@/components/screens/High";
import { Compare } from "@/components/screens/Compare";
import { Telegram } from "@/components/screens/Telegram";
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
    title: "Home · August",
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
    title: "Medium · August",
    position: "column 3 · current",
    note: "One month, day by day. The y axis ends on a round number rather than on the data, so July and August share a scale and can be compared by flicking between them.",
    screen: <Medium data={SAMPLE.medium} />,
  },
  {
    title: "Medium · July",
    position: "column 3 · previous period",
    note: "Same screen, one period back.",
    screen: <Medium data={SAMPLE.mediumPrev} />,
  },
  {
    title: "High · 2026",
    position: "column 4 · current",
    note: "Twelve months, January first. The y axis is fixed rather than fitted, so 2025 and 2026 can be compared without the chart rescaling underneath.",
    screen: <High data={SAMPLE.high} />,
  },
  {
    title: "High · 2025",
    position: "column 4 · previous period",
    note: "January is the year's peak here, which leaves no room above the dot — the label moves alongside it rather than being clipped.",
    screen: <High data={SAMPLE.highPrev} />,
  },
  {
    title: "Compare",
    position: "column 5 · current",
    note: "This home against the average of the comparison group, month for month. The percentage is derived from the two figures, not stored.",
    screen: <Compare data={SAMPLE.compare} />,
  },
  {
    title: "Compare · August",
    position: "column 5 · previous period",
    note: "The same comparison for one closed month.",
    screen: <Compare data={SAMPLE.comparePrev} />,
  },
];

const MAP_ROWS = [
  { label: "n − 1  ↑", cells: ["—", "Home · August", "Medium · July", "High · 2025", "Compare · August"] },
  { label: "current", cells: ["Appliances", "Home", "Medium · August", "High · 2026", "Compare"] },
  { label: "raw  ↓", cells: ["—", "P1 telegram", "—", "—", "—"] },
];

function Cover() {
  const asOf = new Date().toISOString().slice(0, 10);
  return (
    <section className="sheet">
      <div style={{ width: 960 }}>
        <div className="cover-title">PPC Smart Meter Display</div>
        <div className="cover-sub">
          Every screen at 480 × 320, the size of the 3.5 inch panel · {SHEETS.length} screens · as
          of {asOf}
        </div>

        <table className="map">
          <thead>
            <tr>
              <th />
              <th>Appliances</th>
              <th>Home</th>
              <th>Medium</th>
              <th>High</th>
              <th>Compare</th>
            </tr>
          </thead>
          <tbody>
            {MAP_ROWS.map((row) => (
              <tr key={row.label}>
                <td className="row-label">{row.label}</td>
                {row.cells.map((cell, i) => (
                  <td key={i} className={cell === "—" ? "empty" : undefined}>
                    {cell}
                  </td>
                ))}
              </tr>
            ))}
          </tbody>
        </table>

        <div className="legend">
          Sideways, complexity rises left to right. Swipe up goes one period back; swipe down from
          Home reaches the raw telegram. There is no on-screen chrome — the gestures are the whole
          navigation.
          <br />
          <br />
          Each screen below is reproduced at twice its real size, so every pixel of the design sits
          on a whole-number boundary and nothing is resampled. The figures are the sample reading
          the design was drawn against.
        </div>
      </div>
    </section>
  );
}

export default function PrintPage() {
  return (
    <div className="print-root">
      <Cover />
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
