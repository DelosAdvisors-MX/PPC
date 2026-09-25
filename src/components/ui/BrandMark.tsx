/**
 * The PPC mark, served from `public/ppc-logo.png`.
 *
 * A plain <img> rather than next/image: the asset is a fixed 44px on the
 * panel, so there is nothing for the optimizer to decide, and this keeps the
 * screen paintable with no JavaScript in the path. If an SVG of the mark
 * turns up, point `src` at it — nothing else changes.
 */
export function BrandMark({ size = 44 }: { size?: number }) {
  return (
    // eslint-disable-next-line @next/next/no-img-element
    <img
      src="/ppc-logo.png"
      alt="PPC"
      width={size}
      height={size}
      style={{ flexShrink: 0, display: "block" }}
    />
  );
}
