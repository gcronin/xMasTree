#include "lightDefinitions.h"

long timeStamp;
long timeStamp2;
long topTimeStamp;
uint8_t step = 0;
uint8_t stepSecondary = 0;
uint8_t numLit = 0;
boolean newStep = false;
boolean starsInitialized = false;

char colors[3] = {'r', 'g', 'b'};
uint8_t colorIndex = 0;

void takeStep(long rate, uint8_t mod) {
  if(millis() - timeStamp > rate) {
    timeStamp = millis();
    step = (1 + step)%mod;
    newStep = true;
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

void randomFill(boolean StarOn, long rate) {
  boolean newLit = false;
  takeStep(rate, 56);
  changeTopColor(4000);
  if(newStep) {
    while(!newLit) {
      uint8_t Xaddress = random(8);
      uint8_t Yaddress = random(7);
      for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
        if(lights[i]->_xCoor == Xaddress && lights[i]->_yCoor == Yaddress) {
          if(lights[i]->litStatus == false) {
            lights[i]->litStatus = true;
            newLit = true;
            numLit++;
          }
          break;
        }
      }
    }
    newStep = false;
  }
  for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
    if(lights[i]->litStatus == true) {
    lights[i]->TurnOn();
    delayMicroseconds(100);
    lights[i]->TurnOff();
    }
    if(StarOn) TopOn(colors[colorIndex]);
  }
  if(numLit >= 46) {
    numLit = 0;
    for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
      lights[i]->litStatus = false;
    }
  }
}

void starBursts(boolean StarOn, long rate)
{
  uint8_t numberLit = 0;
  const uint8_t numStars = 3;
  takeStep(rate, 4);
  changeTopColor(4000);  
  static uint8_t centerXYcoordinates[2*numStars];
  static boolean stepOneTaken = false;
  static boolean stepTwoTaken = false;
  
  // pick a certain num of center points and mark them to be turned on
  while(!starsInitialized && numberLit < numStars) {
    uint8_t Xaddress = random(8);
    uint8_t Yaddress = random(7);
    for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
      if(lights[i]->_xCoor == Xaddress && lights[i]->_yCoor == Yaddress) {
        if(lights[i]->litStatus == false) {
          lights[i]->litStatus = true;
          centerXYcoordinates[2*numberLit] = Xaddress;
          centerXYcoordinates[2*numberLit+1] = Yaddress;
          numberLit++;
          if(numberLit == numStars) {starsInitialized = true;}
        }
      }
    }
  }

  // do something based on num of steps taken
  switch(step) {
    // just light the center of stars while in the first step
    case 0:
      break;
    
    // expand to nearest neighbors above/below/left/right
    case 1:
      if(!stepOneTaken) {
        for(uint8_t i = 0; i < numStars; i++) {    
          uint8_t newXCoor = (centerXYcoordinates[2*i] + 1)%8;
          //check light with new Xcoor... to the right
          for (size_t j = 0; j < sizeof lights / sizeof lights[0]; j++) {
            if(lights[j]->_xCoor == newXCoor && lights[j]->_yCoor == centerXYcoordinates[2*i+1]) {
              lights[j]->litStatus = true;
            }
          }
        }
      
        for(uint8_t i = 0; i < numStars; i++) {
          uint8_t newYCoor = constrain(centerXYcoordinates[2*i+1] + 1, 0, 6);
            //check light with new Ycoor...  above
          for (size_t j = 0; j < sizeof lights / sizeof lights[0]; j++) {
            if(lights[j]->_xCoor == centerXYcoordinates[2*i] && lights[j]->_yCoor == newYCoor) {
              lights[j]->litStatus = true;
            }
          }
        }
        for(uint8_t i = 0; i < numStars; i++) {
          int newXCoor = (centerXYcoordinates[2*i] - 1);
          if(newXCoor == -1) {newXCoor = 7;}
          //check light with new Xcoor... to the left
          for (size_t j = 0; j < sizeof lights / sizeof lights[0]; j++) {
            if(lights[j]->_xCoor == newXCoor && lights[j]->_yCoor == centerXYcoordinates[2*i+1]) {
              lights[j]->litStatus = true;
            }
          }
        }
        for(uint8_t i = 0; i < numStars; i++) {
          int newYCoor = constrain(centerXYcoordinates[2*i+1] - 1, 0, 6);
            //check light with new Ycoor...  below
          for (size_t j = 0; j < sizeof lights / sizeof lights[0]; j++) {
            if(lights[j]->_xCoor == centerXYcoordinates[2*i] && lights[j]->_yCoor == newYCoor) {
              lights[j]->litStatus = true;
            }
          }
        }
        stepOneTaken = true;
      }
      break;
    
    // expand to diagonal neighbors (above and to the right, above and to the left, etc.)
    case 2:
      if(!stepTwoTaken) {
        // above and to the right
        for(uint8_t i = 0; i < numStars; i++) { 
          uint8_t newXCoor = (centerXYcoordinates[2*i] + 1)%8;
          uint8_t newYCoor = constrain(centerXYcoordinates[2*i+1] + 1, 0, 6);
          for (size_t j = 0; j < sizeof lights / sizeof lights[0]; j++) {
            if(lights[j]->_xCoor == newXCoor && lights[j]->_yCoor == newYCoor) {
              lights[j]->litStatus = true;
            }
          }
        }

        // above and to the left
        for(uint8_t i = 0; i < numStars; i++) { 
          uint8_t newXCoor = (centerXYcoordinates[2*i] + 1)%8;
          int newYCoor = constrain(centerXYcoordinates[2*i+1] - 1, 0, 6);
          for (size_t j = 0; j < sizeof lights / sizeof lights[0]; j++) {
            if(lights[j]->_xCoor == newXCoor && lights[j]->_yCoor == newYCoor) {
              lights[j]->litStatus = true;
            }
          }
        }

        // below and to the right
        for(uint8_t i = 0; i < numStars; i++) { 
          int newXCoor = (centerXYcoordinates[2*i] - 1);
          if(newXCoor == -1) {newXCoor = 7;}
          uint8_t newYCoor = constrain(centerXYcoordinates[2*i+1] + 1, 0, 6);
          for (size_t j = 0; j < sizeof lights / sizeof lights[0]; j++) {
            if(lights[j]->_xCoor == newXCoor && lights[j]->_yCoor == newYCoor) {
              lights[j]->litStatus = true;
            }
          }
        }

        // below and to the left
        for(uint8_t i = 0; i < numStars; i++) { 
          int newXCoor = (centerXYcoordinates[2*i] - 1);
          if(newXCoor == -1) {newXCoor = 7;}
          int newYCoor = constrain(centerXYcoordinates[2*i+1] - 1, 0, 6);
          for (size_t j = 0; j < sizeof lights / sizeof lights[0]; j++) {
            if(lights[j]->_xCoor == newXCoor && lights[j]->_yCoor == newYCoor) {
              lights[j]->litStatus = true;
            }
          }
        }
        stepTwoTaken = true;
      }
      break;
    
    // reset to initial
    case 3:
      starsInitialized = false;
      stepOneTaken = false;
      stepTwoTaken = false;
      for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
        lights[i]->litStatus = false;
      }
      step = 0;
      break;
  }

  // turn on all the lights that are marked
  if(starsInitialized) {
    for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
      if(lights[i]->litStatus == true) {
      lights[i]->TurnOn();
      delayMicroseconds(100);
      lights[i]->TurnOff();
      }
      if(StarOn) TopOn(colors[colorIndex]);
    }
  }
}
