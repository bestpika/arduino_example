#include "Wiegand.h"

#define BAUD 115200
#define PIN0 2
#define PIN1 3

#define KBON true

#if defined(ARDUINO) && ARDUINO >= 100 && KBON
#include "Keyboard.h"
#endif

Wiegand wg;

void setup() {
#if !KBON
  Serial.begin(BAUD);
#endif

  // wg.begin();
  wg.begin(PIN0, PIN1);

#if defined(ARDUINO) && ARDUINO >= 100 && KBON
  Keyboard.begin();
#endif
}

void loop() {
  if (wg.available()) {
    String value = wg.getNumber();

#if !KBON
    Serial.print(value);
#endif
#if defined(ARDUINO) && ARDUINO >= 100 && KBON
    Keyboard.println(value);
#endif
  }
}
