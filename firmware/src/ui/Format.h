#pragma once
#include <stddef.h>

/**
 * European number formatting: comma for the decimal mark, dot for thousands.
 * Ports src/lib/format.ts. Written by hand because embedded has no locale,
 * and callers pass their own buffer so nothing allocates mid-draw.
 */
namespace fmt {

/** "0,84" */
const char* decimal(char* buf, size_t size, float value, int places = 1);
/** "1.812" */
const char* thousands(char* buf, size_t size, float value);
/** "1.812 kWh" */
const char* kwh(char* buf, size_t size, float value);
/** "21,0 kWh" — averages always carry a decimal. */
const char* kwh_decimal(char* buf, size_t size, float value, int places = 1);

/** How far above the comparison group this home sits, as a whole percent. */
int percent_higher(float mine, float theirs);
/** Share of an estimate, as a whole percent. */
int percent_of(float used, float estimate);

}  // namespace fmt
