#include <Arduino.h>

#include "app/App.h"

namespace {
App app;
}  // namespace

void setup() {
  app.begin();
}

void loop() {
  // Everything runs in tasks. Yield rather than spin.
  vTaskDelay(pdMS_TO_TICKS(1000));
}
