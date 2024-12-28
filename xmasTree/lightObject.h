#ifndef __lightObject_H__
#define __lightObject_H__
#include "Arduino.h"

class Light {
  public:
    Light(uint8_t xCoor, uint8_t yCoor);
    void TurnOn();
    void TurnOff();
    uint8_t _xCoor;
    uint8_t _yCoor;
  
  private:
    uint8_t columnPin;
    uint8_t ringPin;
    boolean litStatus;
    uint8_t columnPins[8] = {9, 10, 14, 15, 16, 17, 18, 19};
    uint8_t ringPins[7] = {11, 12, 4, 5, 6, 7, 8};

};
#endif