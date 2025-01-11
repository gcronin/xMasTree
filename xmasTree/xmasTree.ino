#include "lightObject.h"
#include "lightFunctionDefinitions.h"

uint8_t buttonPin = 2;
uint8_t soundSensePin = 3;
uint8_t redLEDPin = 13; 
volatile uint8_t mode = 6;
volatile boolean rotate = false;
static uint8_t numModes = 8;
long modeTimer;


void setup() {
  pinMode(redLEDPin, OUTPUT);
  digitalWrite(redLEDPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), incrementMode, LOW);
  pinMode(soundSensePin, INPUT);
  timeStamp = millis();
  timeStamp2 = millis();
  topTimeStamp = millis();
  modeTimer = millis();
  Serial.begin(9600);
}

void incrementMode() {
    //Static variable initialized only first time increment called, persists between calls.
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200msec, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    if(rotate == true) {
      mode = 0;
      rotate = false;
    }
    else {
      mode = (mode + 1);
      if(mode == numModes) {
        mode = 0;
        rotate = true;
        // this step below is required to prevent getting stuck in randomFill looking for an unlit light to turn on and not finding one
        for (size_t i = 0; i < sizeof lights / sizeof lights[0]; i++) {
          lights[i]->litStatus = false;
        }
      }
    }
  }
  last_interrupt_time = interrupt_time; 
 
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
    case 6:
      randomFill(true, 50);
      break;
    case 7:
      starBursts(true, 150);
      break;
  }

  if(rotate) {
    if(millis() - modeTimer > 10000) {
      modeTimer = millis();
      mode = (mode+1)%numModes;
    }
    digitalWrite(redLEDPin, HIGH);
  }
  else { digitalWrite(redLEDPin, LOW); }
}