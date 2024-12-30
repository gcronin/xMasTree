#include "lightObject.h"
#include "lightFunctionDefinitions.h"

uint8_t buttonPin = 2;
uint8_t soundSensePin = 3;
uint8_t redLEDPin = 13; 



void setup() {
  pinMode(redLEDPin, OUTPUT);
  digitalWrite(redLEDPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(soundSensePin, INPUT);
  timeStamp = millis();

}

void loop() {
  Alternating(false, 1000);

}