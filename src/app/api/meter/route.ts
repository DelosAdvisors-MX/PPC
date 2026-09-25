import { NextResponse } from "next/server";
import { SAMPLE } from "@/lib/data";
import type { MeterSnapshot } from "@/lib/types";

export const dynamic = "force-dynamic";

/**
 * The panel's only data source.
 *
 * Today it returns the design's sample reading. Point `readMeter` at the P1
 * bridge — an ESP32 pushing DSMR frames, a Home Assistant sensor, whatever
 * the install has — and every screen follows without further changes.
 */
async function readMeter(): Promise<MeterSnapshot> {
  return SAMPLE;
}

export async function GET() {
  const snapshot = await readMeter();
  return NextResponse.json(snapshot, {
    headers: { "Cache-Control": "no-store" },
  });
}
