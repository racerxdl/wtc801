#include "wtc801.h"

#define SCK  D5
#define MISO D6
#define CS0  D8
#define MOSI D7

WTC801 panel(SCK, MOSI, MISO, CS0);

void onKeyEvent(KeyEvent event, uint8_t val) {
  switch (event) {
    case Slider:
      Serial.print("Slider: ");
      Serial.println(val, DEC);
      break;
    case KeyLeft:
      Serial.print("KeyLeft: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
    case KeyPower:
      Serial.print("KeyPower: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
    case KeyRight:
      Serial.print("KeyRight: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("OK");
  panel.setOnEvent(&onKeyEvent);
  panel.begin();
}

void loop() {
  panel.loop();
}
