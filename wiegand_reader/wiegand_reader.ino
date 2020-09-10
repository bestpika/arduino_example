#include "Wiegand.h"

#define BAUD 115200
#define PIN0 2
#define PIN1 3

Wiegand wg;

void setup() {
  Serial.begin(BAUD);
  // wg.begin();
  wg.begin(PIN0, PIN1);
}

void loop() {
  if (wg.available()) {
    // 做些什麼
  }
}
