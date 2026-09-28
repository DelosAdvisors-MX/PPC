/**
 * A rectangle rounded on the top corners only, square on the bottom.
 *
 * Every bar on the panel wants this: rounded where it ends, flat where it
 * meets the axis. The obvious trick — draw it over-tall and clip at the
 * baseline — depends on `clip-path`, which not every embedded browser
 * honours; where it is ignored the bar grows a rounded foot that hangs below
 * zero. Drawing the shape outright has no such dependency.
 */
export function topRoundedBar(x: number, y: number, width: number, height: number, radius: number): string {
  if (!(height > 0) || !(width > 0)) return "";
  const r = Math.min(radius, width / 2, height);
  const right = x + width;
  const bottom = y + height;
  return [
    `M${x},${bottom}`,
    `L${x},${y + r}`,
    `Q${x},${y} ${x + r},${y}`,
    `L${right - r},${y}`,
    `Q${right},${y} ${right},${y + r}`,
    `L${right},${bottom}`,
    "Z",
  ].join(" ");
}
