#include "lightObject.h"
#include "lightFunctionDefinitions.h"

uint8_t buttonPin = 2;
uint8_t soundSensePin = 3;
uint8_t redLEDPin = 13; 
uint8_t mode = 0;


void setup() {
  pinMode(redLEDPin, OUTPUT);
  digitalWrite(redLEDPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(soundSensePin, INPUT);
  timeStamp = millis();
  timeStamp2 = millis();
}


void loop() {
  switch(mode) {
    case 0:
      Lines(100);
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
      Rings(500);
      break; 
  }
  
  //

}