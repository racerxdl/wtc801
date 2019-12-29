#include "wtc801.h"
#include <SPI.h>

// 8 ms interval. Datasheet specifies minimum 4ms between reads.
#define READ_PERIOD 8

// MAX and MIN are actually 54 and 1, but it is set to 53 and 2 to have better usage
#define SLIDER_MAX 53
#define SLIDER_MIN 2

#define KEY_RIGHT_CODE 0x00
#define KEY_POWER_CODE 0x01
#define KEY_LEFT_CODE  0x02

WTC801::WTC801(int8_t SCK, int8_t MOSI, int8_t MISO, int8_t CS) {
  this->SCK = SCK;
  this->CS = CS;
  this->MOSI = MOSI;
  this->MISO = MISO;
  this->onKeyEvent = NULL;
  this->lastRead = millis();
  this->lastSliderValue = 0;
  this->lastKeyLeft = LOW;
  this->lastKeyRight = LOW;
  this->lastKeyPower = LOW;
}

uint8_t WTC801::read() {
  digitalWrite(CS, LOW);
  uint8_t data = SPI.transfer(0xFF); // Always transfer full 1 bits
  digitalWrite(CS, HIGH);

  return data;
}

void WTC801::begin() {
  pinMode(SCK, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);

  digitalWrite(CS, HIGH);

  SPI.pins(SCK, MISO, MOSI, CS);
  SPI.begin();
  lastRead = millis();
}

void WTC801::setOnEvent(void (*onKeyEvent)(KeyEvent, uint8_t)) {
  this->onKeyEvent = onKeyEvent;
}

void WTC801::resetButtons() {
  uint8_t L = lastKeyLeft, P = lastKeyPower, R = lastKeyRight;

  lastKeyLeft = LOW;
  lastKeyPower = LOW;
  lastKeyRight = LOW;

  if (L && onKeyEvent != NULL) {
    onKeyEvent(KeyLeft, LOW);
  }
  if (P && onKeyEvent != NULL) {
    onKeyEvent(KeyPower, LOW);
  }
  if (R && onKeyEvent != NULL) {
    onKeyEvent(KeyRight, LOW);
  }
}

void WTC801::process(uint8_t data) {
  uint8_t flag = data & 0x80;
  data &= 0x7F;

  if (data == 0x7F) { // No Data
    // Reset all buttons
    resetButtons();
    return;
  }

  if (flag) { // Touch Keys
    processButtons(data);
  } else {    // Slider
    processSlider(data);
  }
}

void WTC801::processSlider(uint8_t slider) {
  // Convert to percent
  uint16_t s = (uint16_t)slider - SLIDER_MIN;
  if (s > SLIDER_MAX) { // Negative
    s = 0;
  }
  s *= 100;
  s /= SLIDER_MAX;
  slider = (uint8_t)s;

  if (lastSliderValue != slider) {
    if (onKeyEvent != NULL) {
      onKeyEvent(Slider, slider);
    }
  }

  lastSliderValue = slider;
}

void WTC801::processButtons(uint8_t buttons) {
  switch (buttons) {
    case KEY_RIGHT_CODE:
      if (!lastKeyRight) {
        if (onKeyEvent != NULL) {
          onKeyEvent(KeyRight, HIGH);
        }
      }
      lastKeyRight = HIGH;
      break;
    case KEY_POWER_CODE:
      if (!lastKeyPower) {
        if (onKeyEvent != NULL) {
          onKeyEvent(KeyPower, HIGH);
        }
      }
      lastKeyPower = HIGH;
      break;
    case KEY_LEFT_CODE:
      if (!lastKeyLeft) {
        if (onKeyEvent != NULL) {
          onKeyEvent(KeyLeft, HIGH);
        }
      }
      lastKeyLeft = HIGH;
      break;
  }
}


void WTC801::loop() {
  if (millis() - lastRead > READ_PERIOD) {
    uint8_t data = read();
    process(data);
    lastRead = millis();
  }

  if (millis() < lastRead) {
    // Overflow
    lastRead = millis();
  }
}