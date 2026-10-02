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
      month: "October",
      estimateKwh: 650,
      usedKwh: 106,
      caption: "So far in October",
      closed: false,
    },
  },

  homePrev: {
    title: "September",
    badge: "closed",
    // No dial on a closed month: there is no instantaneous draw to point at.
    // 612 kWh over 30 days is 0,85 kW held steady, or 20,4 kWh a day.
    averages: { kw: 0.85, kwhPerDay: 20.4 },
    progress: {
      month: "September",
      estimateKwh: 590,
      usedKwh: 612,
      caption: "104% of estimate",
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
    // October is five days old. A month in progress shows only the days it
    // has; the chart does not pad the rest with zeroes.
    title: "October Insight",
    daysInMonth: 31,
    daily: [22.4, 19.8, 24.1, 18.6, 21.1],
  },

  mediumPrev: {
    title: "September Insight",
    daysInMonth: 30,
    daily: [
      21.4, 18.2, 19.6, 22.8, 17.5, 20.1, 23.4, 19.0, 16.8, 21.7, 23.3, 18.9,
      20.5, 22.1, 15.2, 19.8, 21.0, 22.6, 20.3, 18.4, 28.5, 20.9, 19.1, 20.7,
      17.3, 21.5, 21.8, 19.4, 20.0, 20.2,
    ],
  },

  high: {
    // Nine entries, not twelve: the year stops at the last closed month and
    // the chart leaves the rest of the axis empty rather than drawing months
    // that have not happened. September is 612 here and 612 in mediumPrev.
    title: "2026 Insight",
    monthly: [640, 600, 544, 520, 540, 560, 640, 650, 612],
  },

  highPrev: {
    title: "2025 Insight",
    monthly: [690, 655, 600, 548, 565, 585, 668, 672, 585, 580, 600, 662],
  },

  compare: {
    subtitle: "than similar homes, month for month",
    unitLabel: "kWh a month",
    myKwh: 590,
    theirKwh: 465,
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
    myKwh: 612,
    theirKwh: 486,
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
      { code: "1-0:1.8.1(003376.586*kWh)", gloss: "imported, low tariff" },
      { code: "1-0:1.8.2(002774.705*kWh)", gloss: "imported, normal tariff" },
      { code: "1-0:2.8.1(000249.155*kWh)", gloss: "exported, low tariff" },
      { code: "1-0:2.8.2(000234.567*kWh)", gloss: "exported, normal tariff" },
      { code: "0-0:96.14.0(0001)", gloss: "tariff in use" },
      { code: "1-0:1.7.0(000000.840*kW)", gloss: "drawing now" },
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
