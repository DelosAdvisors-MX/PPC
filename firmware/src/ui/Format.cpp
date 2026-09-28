#include "Format.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

namespace fmt {

const char* decimal(char* buf, size_t size, float value, int places) {
  snprintf(buf, size, "%.*f", places, static_cast<double>(value));
  for (char* c = buf; *c != '\0'; ++c) {
    if (*c == '.') *c = ',';
  }
  return buf;
}

const char* thousands(char* buf, size_t size, float value) {
  char digits[16];
  snprintf(digits, sizeof(digits), "%ld", lroundf(value));

  const size_t length = strlen(digits);
  const bool negative = digits[0] == '-';
  const size_t first = negative ? 1 : 0;

  size_t out = 0;
  if (negative && out + 1 < size) buf[out++] = '-';
  for (size_t i = first; i < length; ++i) {
    // A dot every three digits, counted from the right.
    if (i > first && (length - i) % 3 == 0 && out + 1 < size) buf[out++] = '.';
    if (out + 1 < size) buf[out++] = digits[i];
  }
  buf[out] = '\0';
  return buf;
}

const char* kwh(char* buf, size_t size, float value) {
  char number[16];
  thousands(number, sizeof(number), value);
  snprintf(buf, size, "%s kWh", number);
  return buf;
}

const char* kwh_decimal(char* buf, size_t size, float value, int places) {
  char number[16];
  decimal(number, sizeof(number), value, places);
  snprintf(buf, size, "%s kWh", number);
  return buf;
}

int percent_higher(float mine, float theirs) {
  if (theirs <= 0.0f) return 0;
  return static_cast<int>(lroundf((mine / theirs - 1.0f) * 100.0f));
}

int percent_of(float used, float estimate) {
  if (estimate <= 0.0f) return 0;
  return static_cast<int>(lroundf(used / estimate * 100.0f));
}

}  // namespace fmt
