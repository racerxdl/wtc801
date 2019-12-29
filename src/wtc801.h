#ifndef _WTC801_
#define _WTC801_

#include <Arduino.h>

enum KeyEvent {
  Slider,
  KeyLeft,
  KeyRight,
  KeyPower,
};

class WTC801 {
  private:
    void (*onKeyEvent)(KeyEvent, uint8_t) = NULL;
    int8_t SCK, MOSI, MISO, CS;
    long lastRead;

    uint8_t lastSliderValue;
    uint8_t lastKeyLeft;
    uint8_t lastKeyRight;
    uint8_t lastKeyPower;

    uint8_t read();
    void process(uint8_t);
    void processSlider(uint8_t);
    void processButtons(uint8_t);
    void resetButtons();

  public:
    WTC801(int8_t SCK, int8_t MOSI, int8_t MISO, int8_t CS);

    void begin();
    void setOnEvent(void (*onKeyEvent)(KeyEvent, uint8_t));
    void loop();

    inline uint8_t ReadKeyLeft()  { return lastKeyLeft;     }
    inline uint8_t ReadKeyRight() { return lastKeyRight;    }
    inline uint8_t ReadKeyPower() { return lastKeyPower;    }
    inline uint8_t ReadSlider()   { return lastSliderValue; }
};

#endif
