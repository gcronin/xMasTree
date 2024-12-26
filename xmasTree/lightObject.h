#ifndef __lightObject_H__
#define __lightObject_H__
#include "Arduino.h"

class Light {
  public:
    Light(uint8_t xCoor, uint8_t yCoor);
    void TurnOn();
    void TurnOff();
  
  private:
     uint8_t _xCoor;
     uint8_t _yCoor;
     boolean litStatus;

};
#endif
