import type { MeterSnapshot } from "./types";

/**
 * The sample reading the V2 design was drawn against.
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
    live: { kw: 0.84, scaleKw: 5, caption: "Drawing right now" },
    progress: {
      month: "September",
      estimateKwh: 560,
      usedKwh: 467,
      caption: "So far in September",
      closed: false,
    },
  },

  homePrev: {
    title: "August",
    badge: "closed",
    // No dial on a closed month: there is no instantaneous draw to point at.
    // 650 kWh over 31 days is 0,87 kW held steady, or 21,0 kWh a day.
    averages: { kw: 0.87, kwhPerDay: 20.97 },
    progress: {
      month: "August",
      estimateKwh: 640,
      usedKwh: 650,
      caption: "102% of estimate",
      closed: true,
    },
  },

  appliances: {
    headline: "Electric energy consumption",
    intro:
      "This is your estimated power consumption by appliance so far in the year.",
    items: [
      { id: "hotWater", name: "Hot Water", kwh: 1812, share: 22.6 },
      { id: "tumbleDryer", name: "Tumble Dryer", kwh: 991, share: 12.4 },
      { id: "oven", name: "Oven", kwh: 910, share: 11.4 },
      { id: "hob", name: "Hob", kwh: 853, share: 10.7 },
      { id: "lighting", name: "Lighting", kwh: 729, share: 9.1 },
    ],
  },

  medium: {
    title: "August Insight",
    daily: [
      24.6, 17.2, 21.2, 16.6, 20.3, 18.9, 22.5, 26.1, 16.3, 19.5, 16.6, 16.7,
      19.4, 28.6, 23.0, 18.1, 21.0, 23.6, 20.6, 19.2, 29.8, 22.4, 22.9, 18.3,
      17.2, 17.0, 18.5, 28.6, 23.5, 20.7, 21.1,
    ],
  },

  mediumPrev: {
    title: "July Insight",
    daily: [
      19.0, 19.7, 27.7, 24.5, 19.4, 19.9, 17.1, 19.4, 20.2, 26.8, 22.0, 18.0,
      16.5, 21.5, 20.7, 16.1, 28.1, 28.0, 20.4, 20.1, 16.9, 16.0, 19.5, 21.7,
      22.6, 17.5, 16.1, 19.1, 18.9, 21.7, 24.9,
    ],
  },

  high: {
    title: "2026 Insight",
    monthly: [640, 600, 544, 520, 540, 560, 640, 650, 560, 560, 580, 640],
  },

  highPrev: {
    title: "2025 Insight",
    monthly: [690, 655, 600, 548, 565, 585, 668, 672, 585, 580, 600, 662],
  },

  compare: {
    subtitle: "than similar homes, month for month",
    unitLabel: "kWh a month",
    myKwh: 586,
    theirKwh: 463,
    chips: [
      "Over 140 m²",
      "3 or more people",
      "Built before 1980",
      "Climate zone B",
    ],
    groupSize: 1982,
  },

  comparePrev: {
    eyebrow: "August",
    subtitle: "than similar homes last month",
    unitLabel: "kWh in August",
    myKwh: 650,
    theirKwh: 512,
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
      { code: "0-0:1.0.0(260925163155W)", gloss: "reading time" },
      { code: "1-0:1.8.1(003376.586*kWh)", gloss: "imported, low tariff" },
      { code: "1-0:1.8.2(002774.705*kWh)", gloss: "imported, normal tariff" },
      { code: "1-0:2.8.1(000249.155*kWh)", gloss: "exported, low tariff" },
      { code: "1-0:2.8.2(000234.567*kWh)", gloss: "exported, normal tariff" },
      { code: "0-0:96.14.0(0001)", gloss: "tariff in use" },
      { code: "1-0:1.7.0(000000.840*kW)", gloss: "drawing now" },
      { code: "1-0:2.7.0(000000.000*kW)", gloss: "delivering now" },
      { code: "1-0:32.7.0(230.1*V)", gloss: "voltage" },
    ],
    // Drawn in mint so the eye lands on the figure the Home gauge shows.
    highlight: "1-0:1.7.0(000000.840*kW)",
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
