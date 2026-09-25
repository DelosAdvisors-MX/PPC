/**
 * The display is European: comma for the decimal mark, dot for thousands.
 * Doing this by hand rather than via Intl keeps the output identical
 * regardless of the locale the panel's browser happens to boot with.
 */

export function decimal(value: number, places = 1): string {
  return value.toFixed(places).replace(".", ",");
}

export function thousands(value: number): string {
  return Math.round(value)
    .toString()
    .replace(/\B(?=(\d{3})+(?!\d))/g, ".");
}

/** "1.812 kWh" */
export function kwh(value: number): string {
  return `${thousands(value)} kWh`;
}

/** "21,0 kWh" — used for averages, which always carry one decimal. */
export function kwhDecimal(value: number, places = 1): string {
  return `${decimal(value, places)} kWh`;
}

/** How far above the comparison group this home sits, as a whole percent. */
export function percentHigher(mine: number, theirs: number): number {
  return Math.round((mine / theirs - 1) * 100);
}

/** Share of an estimate, as a whole percent. */
export function percentOf(used: number, estimate: number): number {
  return Math.round((used / estimate) * 100);
}
