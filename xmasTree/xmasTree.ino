#include "lightObject.h"

uint8_t buttonPin = 2;
uint8_t soundSensePin = 3;
uint8_t redLEDPin = 13; 

Light light00 = Light(0,0);

void setup() {
  pinMode(redLEDPin, OUTPUT);
  digitalWrite(redLEDPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(soundSensePin, INPUT);
  light00.TurnOn();
}

void loop() {

}