#include "Arduino.h"
#include "lightObject.h"

Light::Light(uint8_t xCoor, uint8_t yCoor) {
  litStatus = false;
  _xCoor = xCoor;
  _yCoor = yCoor;
}

void Light::TurnOn() {
  //digitalWrite(_pin, HIGH);
}