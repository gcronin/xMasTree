#include "lightObject.h"
#include "lightFunctionDefinitions.h"

uint8_t buttonPin = 2;
uint8_t soundSensePin = 3;
uint8_t redLEDPin = 13; 
uint8_t mode = 0;
uint8_t numModes = 6;
long modeTimer;


void setup() {
  pinMode(redLEDPin, OUTPUT);
  digitalWrite(redLEDPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(soundSensePin, INPUT);
  timeStamp = millis();
  timeStamp2 = millis();
  topTimeStamp = millis();
  modeTimer = millis();
}


void loop() {
  switch(mode) {
    case 0:
      Lines(true, 100, 2000);
      break;
    case 1:
      AllOn(true, 1000);
      break;
    case 2:
      Alternating(true, 250, 3);
      break;
    case 3:
      Columns(true, 50);
      break;
    case 4:
      Rings(200);
      break; 
    case 5:
      Alternating(true, 400, 2);
      break;
  }

  if(millis() - modeTimer > 10000) {
    modeTimer = millis();
    mode = (mode+1)%numModes;
  }
}