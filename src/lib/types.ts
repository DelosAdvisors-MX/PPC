/** Shapes every screen reads from. One reading = one render of the panel. */

export interface LiveReading {
  /** Instantaneous draw in kW, as the P1 port reports it. */
  kw: number;
  /** Full-scale of the gauge in kW. */
  scaleKw: number;
  /** Label under the big number, e.g. "Drawing right now". */
  caption: string;
}

/** What a closed month shows in place of the live dial. */
export interface PeriodAverages {
  /** Mean draw across the whole month, in kW. */
  kw: number;
  /** Mean consumption per day, in kWh. */
  kwhPerDay: number;
}

export interface MonthProgress {
  /** Calendar month the fill bar is measuring. */
  month: string;
  /** Forecast for the whole month, in kWh. */
  estimateKwh: number;
  /** Consumed so far (or in total, once the month is closed), in kWh. */
  usedKwh: number;
  /** Caption under the bar, e.g. "So far in September". */
  caption: string;
  /** A closed month draws the 100% line over the bar and turns magenta. */
  closed: boolean;
}

export interface HomeScreenData {
  /** "My Energy Coach" on the live screen, "August" on the closed one. */
  title: string;
  /** "CLOSED" badge, only on a past month. */
  badge?: string;
  /**
   * The dial, on the live screen only. A closed month has no instantaneous
   * reading to point at, so it carries `averages` instead.
   */
  live?: LiveReading;
  /** Set on a closed month, where averages replace the dial. */
  averages?: PeriodAverages;
  progress: MonthProgress;
}

export interface Appliance {
  id: "hotWater" | "tumbleDryer" | "oven" | "hob" | "lighting";
  name: string;
  kwh: number;
  /** Share of the year's total consumption, in percent. */
  share: number;
}

export interface DailyInsight {
  /** "October Insight" */
  title: string;
  /** One entry per day so far. A month in progress sends fewer than it has. */
  daily: number[];
  /** Days the month holds, so a month in progress keeps its true width. */
  daysInMonth: number;
}

export interface YearInsight {
  /** "2026 Insight" */
  title: string;
  /** Twelve entries, January first. */
  monthly: number[];
}

export interface CompareData {
  /** "AUGUST" eyebrow on the closed-month variant. */
  eyebrow?: string;
  /** "than similar homes, month for month" */
  subtitle: string;
  /** Heading over the chart, e.g. "kWh a month". */
  unitLabel: string;
  myKwh: number;
  theirKwh: number;
  /** The filters that define the comparison group. */
  chips: string[];
  /** How many homes are in that group. */
  groupSize: number;
}

export interface MeterSnapshot {
  home: HomeScreenData;
  homePrev: HomeScreenData;
  appliances: { headline: string; intro: string; items: Appliance[] };
  medium: DailyInsight;
  mediumPrev: DailyInsight;
  high: YearInsight;
  highPrev: YearInsight;
  compare: CompareData;
  comparePrev: CompareData;
  telegram: TelegramData;
}

/** One OBIS line from the meter, with what it means in plain words. */
export interface TelegramLine {
  /** The OBIS reference exactly as the meter sent it. */
  code: string;
  /** What that line is, e.g. "drawing now". */
  gloss: string;
}

export interface TelegramData {
  /** The lines worth showing, not the whole frame. */
  lines: TelegramLine[];
  /** Code prefix to pick out in mint — normally the instantaneous draw. */
  highlight: string;
}
