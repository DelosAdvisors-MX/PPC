import type { NextConfig } from "next";

const nextConfig: NextConfig = {
  reactStrictMode: true,
  // The panel is offline most of the time; keep the client bundle small and
  // never ship source maps to a 3.5 inch screen.
  productionBrowserSourceMaps: false,
  // The panel has no room for the dev overlay; it sits on top of the design.
  devIndicators: false,
};

export default nextConfig;
