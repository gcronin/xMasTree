#include "lightDefinitions.h"

long timeStamp;
boolean step = 1;

char colors[3] = {'r', 'g', 'b'};
uint8_t colorIndex = 0;

void TopOn(char color) {
  switch (color) {
    case 'r':
      PORTC = B00100100;  //14 LOW, 16 & 19 HIGH
      break;
    case 'g':
      PORTC = B00100001;  //16 LOW, 14 & 19 HIGH
      break;
    case 'b':
      PORTC = B00000101;  //19 LOW, 14 & 16 HIGH
      break;
  }
  PORTB |= (1 << 7);
  delayMicroseconds(100);
  PORTB &= (0 << 7);
  PORTC = B00000000;
}

void AllOn(boolean StarOn, long rate) {
  for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
    lights[i]->TurnOn();
    delayMicroseconds(100);
    lights[i]->TurnOff();
    if(StarOn) TopOn(colors[colorIndex]);
  }
  
  
  if(millis() - timeStamp > rate) {
    timeStamp = millis();
    if(StarOn) colorIndex = (colorIndex + 1)%3;
  }
}

void Alternating(boolean StarOn, long rate) {
  if(millis() - timeStamp > rate) {
    timeStamp = millis();
    step = 1- step;
    if(StarOn) colorIndex = (colorIndex + 1)%3;
  }

  if(step == 1) {
    for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
      if((lights[i]->_xCoor + lights[i]->_yCoor)%2== 0) {
        lights[i]->TurnOn();
        delayMicroseconds(100);
        lights[i]->TurnOff();
        if(StarOn) TopOn(colors[colorIndex]);
      }
    }
  }
  else {
    for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
      if((lights[i]->_xCoor + lights[i]->_yCoor)%2== 1) {
        lights[i]->TurnOn();
        delayMicroseconds(100);
        lights[i]->TurnOff();
        if(StarOn) TopOn(colors[colorIndex]);
      }
    }
  }

  
}

