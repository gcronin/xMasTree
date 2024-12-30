#include "lightDefinitions.h"

long timeStamp;
long timeStamp2;
uint8_t step = 0;
uint8_t stepSecondary = 0;

char colors[3] = {'r', 'g', 'b'};
uint8_t colorIndex = 0;

void takeStep(boolean StarOn, long rate, uint8_t mod) {
  if(millis() - timeStamp > rate) {
    timeStamp = millis();
    step = (1 + step)%mod;
    if(StarOn) colorIndex = (colorIndex + 1)%3;
  }
}

void takeSecondaryStep(long rate, uint8_t mod) {
  if(millis() - timeStamp2 > rate) {
    timeStamp2 = millis();
    stepSecondary = (1 + stepSecondary)%mod;
  }
}

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

void Alternating(boolean StarOn, long rate, uint8_t mod) {
  takeStep(StarOn, rate, mod);

  for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
    if((lights[i]->_xCoor + lights[i]->_yCoor)%mod== step) {
      lights[i]->TurnOn();
      delayMicroseconds(100);
      lights[i]->TurnOff();
      if(StarOn) TopOn(colors[colorIndex]);
    }
  }
}

void Columns(boolean StarOn, long rate) {
  takeStep(StarOn, rate, 8);

  for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
    if(lights[i]->_xCoor == step) {
      lights[i]->TurnOn();
      delayMicroseconds(100);
      lights[i]->TurnOff();
      if(StarOn) TopOn(colors[colorIndex]);
    }
  }
}

void Rings(long rate) {
  takeStep(true, rate, 8);

  if(step == 7) {
    TopOn(colors[colorIndex]);
  }
  else {
    for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
      if(lights[i]->_yCoor == step) {
        lights[i]->TurnOn();
        delayMicroseconds(100);
        lights[i]->TurnOff();
      }
    }
  }
}

void Lines1(long rate) {
  takeStep(true, rate, 6);

  for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
    if(lights[i]->_yCoor - lights[i]->_xCoor == step) {
      lights[i]->TurnOn();
      delayMicroseconds(100);
      lights[i]->TurnOff();
    }
  }
}

void Lines2(long rate) {
  takeStep(true, rate, 6);

  for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
    if(lights[i]->_xCoor - lights[i]->_yCoor == step) {
      lights[i]->TurnOn();
      delayMicroseconds(100);
      lights[i]->TurnOff();
    }
  }
}

void Lines(long rate) {
  boolean turnOn = false;
  takeStep(true, rate, 6);
  takeSecondaryStep(2500, 3);

  for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
    switch(stepSecondary) {
      case 0:
        if(lights[i]->_xCoor - lights[i]->_yCoor == step) {turnOn = true;}
        break;
      case 1:
        if(lights[i]->_yCoor - lights[i]->_xCoor == step) {turnOn = true;}
        break;
      case 2: 
        if((lights[i]->_yCoor)*(lights[i]->_yCoor) + lights[i]->_xCoor < step) {turnOn = true;}
        break;
    }
    if(turnOn) {
      lights[i]->TurnOn();
      delayMicroseconds(100);
      lights[i]->TurnOff();
      turnOn = false;
    }
  }
}
