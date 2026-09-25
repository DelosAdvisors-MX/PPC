import type { Metadata, Viewport } from "next";
import { Source_Sans_3, IBM_Plex_Mono } from "next/font/google";
import "./globals.css";

// Self-hosted at build time, so the panel never waits on Google to paint.
// "Ping LCG" is the brand face and is listed ahead of these in the CSS stack;
// drop the licensed file into src/app/fonts and load it with next/font/local
// when it is available.
const sans = Source_Sans_3({
  subsets: ["latin"],
  weight: ["400", "600", "700"],
  variable: "--font-sans",
  display: "swap",
});

const mono = IBM_Plex_Mono({
  subsets: ["latin"],
  weight: ["400", "500"],
  variable: "--font-mono",
  display: "swap",
});

export const metadata: Metadata = {
  title: "PPC Smart Meter Display",
  description: "Live consumption on a 480 x 320, 3.5 inch panel.",
};

export const viewport: Viewport = {
  width: 480,
  height: 320,
  initialScale: 1,
  maximumScale: 1,
  userScalable: false,
  themeColor: "#FFFFFF",
};

export default function RootLayout({ children }: { children: React.ReactNode }) {
  return (
    <html lang="en" className={`${sans.variable} ${mono.variable}`}>
      <body>{children}</body>
    </html>
  );
}
