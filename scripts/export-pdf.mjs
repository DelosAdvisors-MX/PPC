/**
 * Renders /print to a PDF: the cover, then every screen on its own page.
 *
 * Uses the Chrome that is already installed rather than pulling in Playwright
 * and a second browser download — this runs a handful of times a year, not in
 * CI. Set CHROME to override the binary, and PORT if the server is elsewhere.
 */
import { spawn } from "node:child_process";
import { existsSync, mkdirSync } from "node:fs";
import { dirname, resolve } from "node:path";
import { fileURLToPath } from "node:url";

const ROOT = resolve(dirname(fileURLToPath(import.meta.url)), "..");
const PORT = process.env.PORT ?? "3000";
const URL_TO_PRINT = `http://localhost:${PORT}/print`;
const OUTPUT = resolve(ROOT, "docs/ppc-smart-meter-display-screens.pdf");

const CANDIDATES = [
  process.env.CHROME,
  "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome",
  "/Applications/Chromium.app/Contents/MacOS/Chromium",
  "/usr/bin/google-chrome",
  "/usr/bin/chromium",
  "/usr/bin/chromium-browser",
].filter(Boolean);

const chrome = CANDIDATES.find((path) => existsSync(path));
if (!chrome) {
  console.error(`No Chrome found. Tried:\n  ${CANDIDATES.join("\n  ")}\nSet CHROME to its path.`);
  process.exit(1);
}

try {
  const response = await fetch(URL_TO_PRINT, { method: "HEAD" });
  if (!response.ok) throw new Error(`responded ${response.status}`);
} catch (error) {
  console.error(`${URL_TO_PRINT} is not reachable (${error.message}).`);
  console.error("Start the app first: npm run dev");
  process.exit(1);
}

mkdirSync(dirname(OUTPUT), { recursive: true });

const child = spawn(chrome, [
  "--headless=new",
  "--disable-gpu",
  "--no-sandbox",
  // Chrome would otherwise stamp a URL and page numbers over the design.
  "--no-pdf-header-footer",
  // Lets the fonts load and the SVG settle before the page is captured.
  "--virtual-time-budget=20000",
  `--print-to-pdf=${OUTPUT}`,
  URL_TO_PRINT,
]);

// Chrome is noisy on macOS about display links it cannot open when headless.
child.stderr.on("data", (chunk) => {
  const text = String(chunk);
  if (!/CVDisplayLink|ERROR:ui|process_mac/.test(text)) process.stderr.write(text);
});

child.on("close", (code) => {
  if (code === 0) console.log(`wrote ${OUTPUT.replace(`${ROOT}/`, "")}`);
  process.exit(code ?? 1);
});
