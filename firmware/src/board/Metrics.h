#pragma once

/**
 * One design space, two panels.
 *
 * Both panels are 320 design-pixels tall — the 3.5 inch one natively, the
 * 7 inch one at 1.5x — so a single vertical rhythm serves both and only the
 * width changes. The web app in this repo works the same way; the two stay in
 * step because they share the idea, not because anyone remembers to.
 *
 * LVGL has no transform, so every design dimension goes through px(). Fonts
 * are picked per target in ui/Fonts.h: only one target is ever compiled, so
 * only one font set reaches flash — which matters on the 7 inch board, where
 * there are 4MB to work with.
 */
namespace metrics {

#if defined(PANEL_70)
inline constexpr int WIDTH = 800;
inline constexpr int HEIGHT = 480;
inline constexpr float SCALE = 1.5f;
#else
inline constexpr int WIDTH = 480;
inline constexpr int HEIGHT = 320;
inline constexpr float SCALE = 1.0f;
#endif

/** Design pixels to device pixels. */
constexpr int px(float design) {
  return static_cast<int>(design * SCALE + 0.5f);
}

/** The panel's width in design pixels — 480 on the small one, 533 on the big. */
inline constexpr float DESIGN_WIDTH = WIDTH / SCALE;
inline constexpr float DESIGN_HEIGHT = HEIGHT / SCALE;
inline constexpr int PADDING = px(16);

/** Fixed columns the layouts reserve. */
inline constexpr int HOME_COLUMN = px(250);
inline constexpr int HOME_GAP = px(16);
inline constexpr int COMPARE_COLUMN = px(190);
inline constexpr int COMPARE_GAP = px(14);
/** A bordered card's own padding plus its border, both sides. */
inline constexpr int CARD_INSET = px(26);

inline constexpr int CONTENT_WIDTH = WIDTH - PADDING * 2;

/**
 * How wide each chart may draw. Derived rather than hardcoded, exactly as the
 * web app does it, so the charts grow with the panel. On the 3.5 inch panel
 * these come out at 182, 422 and 218 — the figures the design was drawn at.
 */
namespace chart {
inline constexpr int HOME_FILL = CONTENT_WIDTH - HOME_COLUMN - HOME_GAP;
inline constexpr int CARD = CONTENT_WIDTH - CARD_INSET;
inline constexpr int COMPARE = CONTENT_WIDTH - COMPARE_COLUMN - COMPARE_GAP - CARD_INSET;
}  // namespace chart

}  // namespace metrics
