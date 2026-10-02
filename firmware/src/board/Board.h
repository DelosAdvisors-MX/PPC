#pragma once

/**
 * Picks the panel and touch controller for whichever target is being built.
 *
 * A compile-time choice rather than a virtual interface: only one board is ever
 * in a binary, the unused driver's .cpp is excluded by build_src_filter in
 * platformio.ini, and nothing pays for a vtable. Add a third panel by adding a
 * pair of files, a build flag and three lines here.
 */

#include "Metrics.h"

#if defined(PANEL_70)
#include "Panel70.h"
#include "TouchGT911.h"
namespace board {
using ActivePanel = Panel70;
using ActiveTouch = TouchGT911;
inline constexpr const char* TARGET = "panel-70 (7.0in CrowPanel, RGB 800x480)";
}  // namespace board
#elif defined(PANEL_35)
#include "Panel35.h"
#include "TouchAXS.h"
namespace board {
using ActivePanel = Panel35;
using ActiveTouch = TouchAXS;
inline constexpr const char* TARGET = "panel-35 (3.5in Gugxiom, AXS15231B QSPI)";
}  // namespace board
#else
#error "No panel selected. Build with -DPANEL_35 or -DPANEL_70 (see platformio.ini)."
#endif
