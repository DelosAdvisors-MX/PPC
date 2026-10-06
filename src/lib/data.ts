import type { MeterSnapshot } from "./types";

/**
 * The sample reading: a two-person European home, electricity only.
 *
 * Roughly 3.000 kWh a year — in the band a couple without electric heating
 * actually lands in — which is about a third of what this file used to claim.
 * Every figure here is tied to the others, so changing one means changing the
 * rest: September's total is the September point on the year chart and the
 * figure the comparison uses, the appliance shares are shares of the year so
 * far, and the daily series averages to what Home reports.
 *
 * The daily and monthly series were recovered from the artifact's SVG
 * geometry and cross-checked against the totals printed on each screen:
 *   August  daily  -> 650,0 kWh total, 21,0 kWh/day average
 *   July    daily  -> 640,0 kWh total, 20,6 kWh/day average
 *   2026    monthly -> 7.034 kWh total, 586 kWh/month average
 *   2025    monthly -> 7.410 kWh total, 618 kWh/month average
 *
 * Replace this object with a live P1 feed and every screen follows.
 */
export const SAMPLE: MeterSnapshot = {
  home: {
    title: "My Energy Coach",
    live: { kw: 0.41, scaleKw: 5, caption: "Drawing right now" },
    progress: {
      month: "October",
      estimateKwh: 265,
      usedKwh: 43,
      caption: "So far in October",
      closed: false,
    },
  },

  homePrev: {
    title: "September",
    badge: "closed",
    // No dial on a closed month: there is no instantaneous draw to point at.
    // 268 kWh over 30 days is 0,37 kW held steady, or 8,9 kWh a day.
    averages: { kw: 0.37, kwhPerDay: 8.93 },
    progress: {
      month: "September",
      estimateKwh: 255,
      usedKwh: 268,
      caption: "105% of estimate",
      closed: true,
    },
  },

  appliances: {
    headline: "Electric energy consumption",
    intro:
      "This is your estimated power consumption by appliance so far in the year.",
    items: [
      // Shares of the 2.273 kWh the year has used so far.
      { id: "hotWater", name: "Hot Water", kwh: 514, share: 22.6 },
      { id: "tumbleDryer", name: "Tumble Dryer", kwh: 282, share: 12.4 },
      { id: "oven", name: "Oven", kwh: 259, share: 11.4 },
      { id: "hob", name: "Hob", kwh: 243, share: 10.7 },
      { id: "lighting", name: "Lighting", kwh: 207, share: 9.1 },
    ],
  },

  medium: {
    // October is five days old. A month in progress shows only the days it
    // has; the chart does not pad the rest with zeroes.
    title: "October Insight",
    daysInMonth: 31,
    daily: [9.1, 7.8, 10.2, 7.6, 8.3],
  },

  mediumPrev: {
    title: "September Insight",
    daysInMonth: 30,
    daily: [
      10.2, 7.8, 8.4, 9.9, 7.5, 8.6, 10.1, 8.2, 7.2, 9.3, 11.0, 8.1, 8.8, 9.5,
      6.5, 8.5, 9.0, 10.7, 8.7, 7.9, 13.2, 9.0, 8.2, 8.9, 7.4, 9.2, 10.6, 8.3,
      8.6, 8.7,
    ],
  },

  high: {
    // Nine entries, not twelve: the year stops at the last closed month and
    // the chart leaves the rest of the axis empty rather than drawing months
    // that have not happened. September is 612 here and 612 in mediumPrev.
    title: "2026 Insight",
    monthly: [265, 240, 215, 195, 205, 250, 310, 325, 268],
  },

  highPrev: {
    title: "2025 Insight",
    monthly: [272, 248, 220, 198, 208, 255, 318, 332, 275, 232, 240, 262],
  },

  compare: {
    subtitle: "than similar homes, month for month",
    unitLabel: "kWh a month",
    myKwh: 253,
    theirKwh: 199,
    chips: [
      "Over 140 m²",
      "3 or more people",
      "Built before 1980",
      "Climate zone B",
    ],
    groupSize: 1982,
  },

  comparePrev: {
    eyebrow: "September",
    subtitle: "than similar homes last month",
    unitLabel: "kWh in September",
    myKwh: 268,
    theirKwh: 212,
    chips: [
      "Over 140 m²",
      "3 or more people",
      "Built before 1980",
      "Climate zone B",
    ],
    groupSize: 1982,
  },

  telegram: {
    // The frame carries far more than this. These are the lines a household
    // can act on: what the meter has counted, which way it is flowing right
    // now, and whether the voltage is sane. The equipment id, the DSMR
    // version and the message blocks are dropped.
    lines: [
      { code: "0-0:1.0.0(261005163155W)", gloss: "reading time" },
      { code: "1-0:1.8.1(009427.183*kWh)", gloss: "imported, low tariff" },
      { code: "1-0:1.8.2(007812.455*kWh)", gloss: "imported, normal tariff" },
      { code: "1-0:2.8.1(000312.044*kWh)", gloss: "exported, low tariff" },
      { code: "1-0:2.8.2(000198.620*kWh)", gloss: "exported, normal tariff" },
      { code: "0-0:96.14.0(0001)", gloss: "tariff in use" },
      { code: "1-0:1.7.0(000000.410*kW)", gloss: "drawing now" },
      { code: "1-0:2.7.0(000000.000*kW)", gloss: "delivering now" },
      { code: "1-0:32.7.0(230.1*V)", gloss: "voltage" },
    ],
    // A prefix, not the whole line: the value is rewritten every tick.
    highlight: "1-0:1.7.0",
  },
};

export const MONTH_LABELS = [
  "JAN",
  "FEB",
  "MAR",
  "APR",
  "MAY",
  "JUN",
  "JUL",
  "AUG",
  "SEP",
  "OCT",
  "NOV",
  "DEC",
] as const;
