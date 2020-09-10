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
    static bool flagDone;
    static unsigned char dataBits[MAX_BITS];
    static unsigned long counter;
    static unsigned long _bitCount;
    static unsigned long  bitCount;
    // 卡片資料
    static unsigned long _faciCode;
    static unsigned long  faciCode;
    static unsigned long _cardCode;
    static unsigned long  cardCode;
    static unsigned long _fullCode;
    static unsigned long  fullCode;
};

#endif
