#include "Arduino.h"
#include "lightObject.h"

//Constructor:  looks up the appropriate row/column pins, sets those as outputs, turns off the LED
Light::Light(uint8_t xCoor, uint8_t yCoor) {
  columnPin = columnPins[xCoor] ;
  ringPin = ringPins[yCoor];
  pinMode(columnPin, OUTPUT);
  pinMode(ringPin, OUTPUT);
  digitalWrite(columnPin, LOW);
  digitalWrite(ringPin, LOW);
  litStatus = false;
}

void Light::TurnOn() {
  digitalWrite(columnPin, HIGH);
  digitalWrite(ringPin, HIGH);
  litStatus = true;
}

void Light::TurnOff() {
  digitalWrite(columnPin, LOW);
  digitalWrite(ringPin, LOW);
  litStatus = false;
}