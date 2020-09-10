#include "Wiegand.h"

#if   defined(ESP8266)
#define INTERRUPT_ATTR ICACHE_RAM_ATTR
#elif defined(ESP32)
#define INTERRUPT_ATTR IRAM_ATTR
#else
#define INTERRUPT_ATTR
#endif

volatile bool Wiegand::flagDone;
volatile unsigned char Wiegand::dataBits[MAX_BITS];
volatile unsigned long Wiegand::counter;
volatile unsigned long Wiegand::_bitCount;
volatile unsigned long Wiegand:: bitCount;
volatile unsigned long Wiegand::_faciCode;
volatile unsigned long Wiegand:: faciCode;
volatile unsigned long Wiegand::_cardCode;
volatile unsigned long Wiegand:: cardCode;
volatile unsigned long Wiegand::_fullCode;
volatile unsigned long Wiegand:: fullCode;

Wiegand::Wiegand() {}

void Wiegand::begin() {
  begin(2, 3);
}

void Wiegand::begin(int pin0, int pin1) {
  pinMode(pin0, INPUT);
  pinMode(pin1, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin0), ISR_INT0, FALLING);
  attachInterrupt(digitalPinToInterrupt(pin1), ISR_INT1, FALLING);
  // init
  flagDone = false;
  counter = WAIT_TIME;
  _bitCount = 0;
  _faciCode = 0;
  _cardCode = 0;
  _fullCode = 0;
}

bool Wiegand::available() {
  bool ret;
  noInterrupts();
  ret = parseBits();
  interrupts();
  return ret;
}

void Wiegand::printBits() {
  Serial.print("Size = ");
  Serial.print(bitCount);
  Serial.print(", Faci = ");
  Serial.print(faciCode);
  Serial.print(", Card = ");
  Serial.print(cardCode);
  Serial.print(", Full = ");
  Serial.print(fullCode);
  Serial.println();
}

String Wiegand::getNumber() {
  String ans;
  switch (bitCount) {
    case 34:
      ans.concat(fullCode);
      break;
    case 26:
      ans.concat(faciCode);
      ans.concat(",");
      ans.concat(cardCode);
      break;
  }
  return ans;
}

INTERRUPT_ATTR void Wiegand::ISR_INT0() {
  // Serial.print("0");
  _bitCount++;
  flagDone = false;
  counter = WAIT_TIME;
}

INTERRUPT_ATTR void Wiegand::ISR_INT1() {
  // Serial.print("1");
  dataBits[_bitCount++] = 1;
  flagDone = false;
  counter = WAIT_TIME;
}

bool Wiegand::parseBits() {
  if (!flagDone && --counter == 0) {
    flagDone = true;
  }
  if (_bitCount > 0 && flagDone) {
    int i, faci, card;
    if (_bitCount == 34) {
      // facility code [1..12]
      faci = 12;
      // card code [13..32]
      card = 32;
    } else if (_bitCount == 26) {
      // facility code [1..8]
      faci = 8;
      // card code [9..24]
      card = 24;
    } else {
      flagDone = false;
    }
    // 轉換
    if (flagDone) {
      for (i = 1; i <= faci; i++) {
        _faciCode <<= 1;
        _faciCode |= dataBits[i];
      }
      for (i = faci + 1; i <= card; i++) {
        _cardCode <<= 1;
        _cardCode |= dataBits[i];
      }
      for (i = 1; i <= card; i++) {
        _fullCode <<= 1;
        _fullCode |= dataBits[i];
      }
    }
    // 轉存
    bitCount = _bitCount;
    faciCode = _faciCode;
    cardCode = _cardCode;
    fullCode = _fullCode;
    // 清除
    _bitCount = 0;
    _faciCode = 0;
    _cardCode = 0;
    _fullCode = 0;
    for (i = 0; i < MAX_BITS; i++) {
      dataBits[i] = 0;
    }
  } else {
    flagDone = false;
  }
  return flagDone;
}
