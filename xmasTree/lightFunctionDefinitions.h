#include "lightDefinitions.h"

long timeStamp;
long timeStamp2;
long topTimeStamp;
uint8_t step = 0;
uint8_t stepSecondary = 0;

char colors[3] = {'r', 'g', 'b'};
uint8_t colorIndex = 0;

void takeStep(long rate, uint8_t mod) {
  if(millis() - timeStamp > rate) {
    timeStamp = millis();
    step = (1 + step)%mod;
  }
}

void takeSecondaryStep(long rate, uint8_t mod) {
  if(millis() - timeStamp2 > rate) {
    timeStamp2 = millis();
    stepSecondary = (1 + stepSecondary)%mod;
  }
}

void changeTopColor(long rate) {
  if(millis() - topTimeStamp > rate) {
    topTimeStamp = millis();
    colorIndex = (colorIndex + 1)%3;
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
    if(StarOn) {  TopOn(colors[colorIndex]);  }
  }
  changeTopColor(4000);
}

void Alternating(boolean StarOn, long rate, uint8_t mod) {
  takeStep(rate, mod);
  changeTopColor(4000);

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
  takeStep(rate, 8);
  changeTopColor(4000);

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
  takeStep(rate, 8);
  changeTopColor(5000);

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

void Lines(boolean StarOn, long forRate, int caseRate) {
  boolean turnOn = false;
  takeStep(forRate, 6);
  takeSecondaryStep(caseRate, 4);
  changeTopColor(4000);


  for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
    switch(stepSecondary) {
      case 0:
        if(lights[i]->_xCoor - lights[i]->_yCoor == step) {turnOn = true;}
        break;
      case 1:
        if(lights[i]->_yCoor - lights[i]->_xCoor == step) {turnOn = true;}
        break;
      case 2: 
        if((lights[i]->_yCoor)/step == lights[i]->_xCoor) {turnOn = true;}
        break;
      case 3: 
        if((lights[i]->_xCoor)/step == lights[i]->_yCoor) {turnOn = true;}
        break;
    }
    if(turnOn) {
      lights[i]->TurnOn();
      delayMicroseconds(100);
      lights[i]->TurnOff();
      turnOn = false;
    }
    if(StarOn) TopOn(colors[colorIndex]);
  }
}
