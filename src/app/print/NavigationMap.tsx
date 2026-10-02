import type { ReactNode } from "react";
import type { PanelSpec } from "@/lib/panel";

/**
 * The screen map, drawn rather than tabulated: every screen in its place, with
 * a line to each screen you can reach from it.
 *
 * Thumbnails are the real screens at 0.35, so the map doubles as a contact
 * sheet — the shape of a screen is usually enough to recognise it.
 */

/** Thumbnails are a fixed width so five columns always fit the page; their
    height follows the panel's own proportions. */
const THUMB_W = 168;
const GAP_X = 28;
const GAP_Y = 40;
const LABEL_H = 16;
const COL_PITCH = THUMB_W + GAP_X; // 196
const GRID_W = 4 * COL_PITCH + THUMB_W; // 952
const GUTTER = 96; // room for the row labels

export interface MapCell {
  column: number;
  row: number;
  label: string;
  screen: ReactNode;
}

const COLUMNS = ["Appliances", "Home", "Medium", "High", "Compare"];

const ROWS = [
  { title: "n − 1", note: "one period back" },
  { title: "current", note: "entry row" },
  { title: "raw", note: "under Home only" },
];

/** Pairs you can swipe between. Every line is travelled in both directions. */
const HORIZONTAL: Array<[number, number]> = [
  // previous-period row: no appliances tile, so it starts at Home
  [1, 0],
  [2, 0],
  [3, 0],
  // current row, all the way across
  [0, 1],
  [1, 1],
  [2, 1],
  [3, 1],
];

const VERTICAL: Array<[number, number]> = [
  // current up to its previous period
  [1, 0],
  [2, 0],
  [3, 0],
  [4, 0],
  // Home down to the raw telegram
  [1, 1],
];

const thumbX = (column: number) => column * COL_PITCH;

export function NavigationMap({ cells, panel }: { cells: MapCell[]; panel: PanelSpec }) {
  const scale = THUMB_W / panel.width;
  const thumbH = Math.round(panel.height * scale);
  const rowPitch = thumbH + LABEL_H + GAP_Y;
  const gridH = 2 * rowPitch + LABEL_H + thumbH;
  const thumbY = (row: number) => row * rowPitch + LABEL_H;

  return (
    <div className="map-wrap" style={{ width: GUTTER + GRID_W }}>
      <div className="map-complexity" style={{ marginLeft: GUTTER, width: GRID_W }}>
        complexity rises →
      </div>

      <div className="map-columns" style={{ marginLeft: GUTTER, width: GRID_W }}>
        {COLUMNS.map((name, i) => (
          <span key={name} className="map-column" style={{ left: thumbX(i), width: THUMB_W }}>
            {name}
          </span>
        ))}
      </div>

      <div className="map-grid" style={{ marginLeft: GUTTER, width: GRID_W, height: gridH }}>
        <svg
          className="map-lines"
          width={GRID_W}
          height={gridH}
          viewBox={`0 0 ${GRID_W} ${gridH}`}
          aria-hidden="true"
        >
          <defs>
            <marker
              id="arrow"
              viewBox="0 0 8 8"
              refX="7"
              refY="4"
              markerWidth="6"
              markerHeight="6"
              orient="auto-start-reverse"
            >
              <path d="M0,1 L7,4 L0,7 z" fill="#9AA3AD" />
            </marker>
          </defs>

          {HORIZONTAL.map(([column, row]) => {
            const y = thumbY(row) + thumbH / 2;
            return (
              <line
                key={`h${column}-${row}`}
                x1={thumbX(column) + THUMB_W + 5}
                y1={y}
                x2={thumbX(column + 1) - 5}
                y2={y}
                stroke="#9AA3AD"
                strokeWidth={1.5}
                markerStart="url(#arrow)"
                markerEnd="url(#arrow)"
              />
            );
          })}

          {VERTICAL.map(([column, row]) => {
            const x = thumbX(column) + THUMB_W / 2;
            return (
              <line
                key={`v${column}-${row}`}
                x1={x}
                y1={thumbY(row) + thumbH + 5}
                x2={x}
                y2={thumbY(row + 1) - 5}
                stroke="#9AA3AD"
                strokeWidth={1.5}
                markerStart="url(#arrow)"
                markerEnd="url(#arrow)"
              />
            );
          })}
        </svg>

        {ROWS.map((row, i) => (
          <div key={row.title} className="map-row-label" style={{ top: thumbY(i) }}>
            <span className="map-row-title">{row.title}</span>
            <span className="map-row-note">{row.note}</span>
          </div>
        ))}

        {cells.map((cell) => (
          <div
            key={cell.label}
            className="map-cell"
            style={{ left: thumbX(cell.column), top: thumbY(cell.row) - LABEL_H }}
          >
            <span className="map-cell-label" style={{ width: THUMB_W }}>
              {cell.label}
            </span>
            <div className="map-thumb" style={{ width: THUMB_W, height: thumbH }}>
              <div
                style={{
                  width: panel.width,
                  height: panel.height,
                  transform: `scale(${scale})`,
                  transformOrigin: "top left",
                }}
              >
                {cell.screen}
              </div>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}
