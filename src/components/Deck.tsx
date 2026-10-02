"use client";

import { useCallback, useEffect, useRef, useState, type ReactNode } from "react";
import type { PanelSpec } from "@/lib/panel";

/**
 * Navigation follows the artifact's map:
 *
 *   n-1      |            | Home · Aug | Medium · Jul | High · 2025 | Compare · Aug
 *   current  | Appliances | Home       | Medium · Aug | High · 2026 | Compare
 *   raw      |            | P1 telegram|              |             |
 *
 * Sideways, complexity rises left to right. Up goes one period back, down
 * from Home reaches the raw telegram. The gesture direction is the direction
 * you travel — swipe up and the panel moves up the map — which is what the
 * design's own legend describes.
 */
export interface Cell {
  col: number;
  row: number;
  key: string;
  node: ReactNode;
}

interface Props {
  cells: Cell[];
  panel: PanelSpec;
  /** Where to start. Home is (1, 0). */
  start?: { col: number; row: number };
}

/** Pixels of travel before a drag counts as a swipe. */
const THRESHOLD = 45;
/** How far a drag into empty space is allowed to stretch. */
const RESISTANCE = 0.22;

/**
 * The design's legend reads "swipe up: one period back" with n-1 drawn above
 * Home, and "swipe down from Home" for the telegram drawn below it — so the
 * gesture points where you are going, like a D-pad, and the deck slides
 * against your finger. Set this to -1 for the other convention, where the
 * finger drags the deck itself and swiping up reveals what sits below.
 */
const GESTURE = 1;

export function Deck({ cells, panel, start = { col: 1, row: 0 } }: Props) {
  const [pos, setPos] = useState(start);
  const [drag, setDrag] = useState({ x: 0, y: 0 });
  const [dragging, setDragging] = useState(false);

  const origin = useRef<{ x: number; y: number } | null>(null);
  const axis = useRef<"x" | "y" | null>(null);
  /** Raw finger travel, kept so the release reads the gesture, not the offset. */
  const delta = useRef({ dx: 0, dy: 0 });

  const at = useCallback(
    (col: number, row: number) => cells.some((c) => c.col === col && c.row === row),
    [cells],
  );

  /**
   * Sideways keeps the current row when the target column has one; otherwise
   * it drops to the current row, which is what the map implies for Appliances.
   */
  const target = useCallback(
    (dx: number, dy: number) => {
      if (dx !== 0) {
        const col = pos.col + dx;
        if (at(col, pos.row)) return { col, row: pos.row };
        if (at(col, 0)) return { col, row: 0 };
        return null;
      }
      const row = pos.row + dy;
      return at(pos.col, row) ? { col: pos.col, row } : null;
    },
    [at, pos],
  );

  const go = useCallback(
    (dx: number, dy: number) => {
      const next = target(dx, dy);
      if (next) setPos(next);
    },
    [target],
  );

  useEffect(() => {
    const onKey = (e: KeyboardEvent) => {
      const moves: Record<string, [number, number]> = {
        ArrowLeft: [-1, 0],
        ArrowRight: [1, 0],
        ArrowUp: [0, -1],
        ArrowDown: [0, 1],
      };
      const move = moves[e.key];
      if (!move) return;
      e.preventDefault();
      go(move[0], move[1]);
    };
    window.addEventListener("keydown", onKey);
    return () => window.removeEventListener("keydown", onKey);
  }, [go]);

  const onPointerDown = (e: React.PointerEvent) => {
    e.currentTarget.setPointerCapture(e.pointerId);
    origin.current = { x: e.clientX, y: e.clientY };
    axis.current = null;
    setDragging(true);
  };

  const onPointerMove = (e: React.PointerEvent) => {
    if (!origin.current) return;
    const dx = e.clientX - origin.current.x;
    const dy = e.clientY - origin.current.y;

    // Lock to whichever axis the finger committed to first.
    if (!axis.current && Math.hypot(dx, dy) > 8) {
      axis.current = Math.abs(dx) > Math.abs(dy) ? "x" : "y";
    }
    if (!axis.current) return;

    delta.current = { dx, dy };

    // The screen you are heading for slides in from the side you swiped
    // toward, so the deck travels against the finger.
    if (axis.current === "x") {
      const open = target(dx > 0 ? 1 : -1, 0);
      const travel = -dx * GESTURE;
      setDrag({ x: open ? travel : travel * RESISTANCE, y: 0 });
    } else {
      const open = target(0, dy > 0 ? 1 : -1);
      const travel = -dy * GESTURE;
      setDrag({ x: 0, y: open ? travel : travel * RESISTANCE });
    }
  };

  const onPointerUp = () => {
    const { dx, dy } = delta.current;
    if (axis.current === "x" && Math.abs(dx) > THRESHOLD) {
      go(dx * GESTURE > 0 ? 1 : -1, 0);
    } else if (axis.current === "y" && Math.abs(dy) > THRESHOLD) {
      go(0, dy * GESTURE > 0 ? 1 : -1);
    }

    origin.current = null;
    axis.current = null;
    delta.current = { dx: 0, dy: 0 };
    setDrag({ x: 0, y: 0 });
    setDragging(false);
  };

  const offsetX = -pos.col * panel.width + drag.x;
  const offsetY = -pos.row * panel.height + drag.y;

  return (
    <div
      className="deck-viewport"
      onPointerDown={onPointerDown}
      onPointerMove={onPointerMove}
      onPointerUp={onPointerUp}
      onPointerCancel={onPointerUp}
      style={{ width: panel.width, height: panel.height }}
    >
      <div
        className="deck-grid"
        style={{
          transform: `translate3d(${offsetX}px, ${offsetY}px, 0)`,
          transition: dragging ? "none" : "transform 260ms cubic-bezier(.22,.61,.36,1)",
        }}
      >
        {cells.map((cell) => {
          const current = cell.col === pos.col && cell.row === pos.row;
          return (
            <div
              key={cell.key}
              className="deck-cell"
              aria-hidden={!current}
              style={{
                left: cell.col * panel.width,
                top: cell.row * panel.height,
                width: panel.width,
                height: panel.height,
              }}
            >
              {cell.node}
            </div>
          );
        })}
      </div>
    </div>
  );
}
