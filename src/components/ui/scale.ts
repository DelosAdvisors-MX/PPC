/** Rounds a rough step up to something a reader can count in: 1, 2, 2.5, 5, 10... */
function niceStep(rough: number): number {
  const magnitude = 10 ** Math.floor(Math.log10(rough));
  for (const factor of [1, 2, 2.5, 5, 10]) {
    if (factor * magnitude >= rough) return factor * magnitude;
  }
  return 10 * magnitude;
}

/**
 * An axis that starts at zero and ends on a round number above the data, with
 * roughly `intervals` gaps. Two months of similar size land on the same axis,
 * so flicking between them compares like with like.
 */
export function zeroBasedTicks(peak: number, intervals = 3): { max: number; ticks: number[] } {
  if (!(peak > 0)) return { max: 1, ticks: [0, 1] };
  const step = niceStep(peak / intervals);
  const max = Math.ceil(peak / step) * step;
  const ticks: number[] = [];
  // Built by multiplication rather than accumulation so 0,1 steps stay exact.
  for (let i = 0; i * step <= max + 1e-9; i++) ticks.push(Number((i * step).toFixed(6)));
  return { max, ticks };
}
