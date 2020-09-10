#ifndef WIEGAND_H
#define WIEGAND_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define MAX_BITS  100
#define WAIT_TIME 1000

class Wiegand {
  public:
    Wiegand();
    void begin();
    void begin(int pin0, int pin1);
    bool available();
    void printBits();
    String getNumber();
  protected:
    static void ISR_INT0();
    static void ISR_INT1();
    bool parseBits();
  private:
    static volatile bool flagDone;
    static volatile unsigned char dataBits[MAX_BITS];
    static volatile unsigned long counter;
    static volatile unsigned long _bitCount;
    static volatile unsigned long  bitCount;
    // 卡片資料
    static volatile unsigned long _faciCode;
    static volatile unsigned long  faciCode;
    static volatile unsigned long _cardCode;
    static volatile unsigned long  cardCode;
    static volatile unsigned long _fullCode;
    static volatile unsigned long  fullCode;
};

#endif
