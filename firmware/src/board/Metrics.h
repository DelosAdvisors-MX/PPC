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

}  // namespace metrics
