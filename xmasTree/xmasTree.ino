#include "lightObject.h"

uint8_t buttonPin = 2;
uint8_t soundSensePin = 3;
uint8_t redLEDPin = 13; 

long timeStamp;
boolean step = 1;

Light light00 = Light(0,0);
Light light01 = Light(0,1);
Light light02 = Light(0,2);
Light light03 = Light(0,3);
Light light04 = Light(0,4);
Light light05 = Light(0,5);
Light light06 = Light(0,6);
Light light10 = Light(1,0);
Light light11 = Light(1,1);
Light light12 = Light(1,2);
Light light13 = Light(1,3);
Light light14 = Light(1,4);
Light light15 = Light(1,5);
Light light16 = Light(1,6);

Light *lights[] = {&light00, &light01, &light02, &light03, &light04, &light05, &light06,
                    &light10, &light11, &light12, &light13, &light14, &light15, &light16
};

void setup() {
  pinMode(redLEDPin, OUTPUT);
  digitalWrite(redLEDPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(soundSensePin, INPUT);
  timeStamp = millis();

}

void loop() {
  if(step == 1) {
    for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
      if((lights[i]->_xCoor + lights[i]->_yCoor)%2== 0) {
        lights[i]->TurnOn();
        delay(1);
        lights[i]->TurnOff();
      }
    }
  }
  else {
    for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
      if((lights[i]->_xCoor + lights[i]->_yCoor)%2== 1) {
        lights[i]->TurnOn();
        delay(1);
        lights[i]->TurnOff();
      }
    }
  }


  if(millis() - timeStamp > 1000) {
    timeStamp = millis();
    step = 1- step;
  }

}