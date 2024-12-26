uint8_t columnPins[8] = {9, 10, 14, 15, 16, 17, 18, 19};
uint8_t ringPins[7] = {11, 12, 4, 5, 6, 7, 8};
uint8_t buttonPin = 2;
uint8_t soundSensePin = 3;
uint8_t redLEDPin = 13; 

char colors[3] = {'r', 'g', 'b'};
uint8_t colorIndex = 0;

long timeStamp;
 
void setup() {
  pinMode(redLEDPin, OUTPUT);
  digitalWrite(redLEDPin, LOW);
  for(uint8_t i = 0; i<8; i++) {
    pinMode(columnPins[i], OUTPUT);
    digitalWrite(columnPins[i], LOW);
  }
  for(uint8_t i = 0; i<7; i++) {
    pinMode(ringPins[i], OUTPUT);
    digitalWrite(ringPins[i], LOW);
  }
  DDRB |= (1 << 7);  //Set PB7 as output
  PORTB &= (0 << 7); //Set PB7 low

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(soundSensePin, INPUT);
  timeStamp = millis();
  
}

void loop() {
  if(millis() - timeStamp > 2000) {
    timeStamp = millis();
    colorIndex = (colorIndex + 1)%3;
  }
  AllOn(50, colors[colorIndex]);
}

void AllOn(uint8_t usDelay, char color) {
  for(uint8_t i = 0; i<8; i++) {
    digitalWrite(columnPins[i], HIGH);
    for(uint8_t i = 0; i<7; i++) {
      digitalWrite(ringPins[i], HIGH);
      delayMicroseconds(usDelay);
      digitalWrite(ringPins[i], LOW);
    }
    digitalWrite(columnPins[i], LOW);
  }
  TopOn(5*usDelay, color);
}

void TopOn(uint8_t usDelay, char color) {
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
  delayMicroseconds(usDelay);
  PORTB &= (0 << 7);
  PORTC = B00000000;
}