const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int pinSw = 2;
const int pinY = A5;
const int pinX = A4;

const byte regSize = 8; // 1 byte aka 8 bits

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};

const int encodingsNumber = 16;

bool isStateTwo = false; //false means state 1, true means state 2
bool isPressing = false;
bool joyMoved = false;
int minThreshold = 30; //thresholds are closer to the min/max values to prevent seeing the press of a button as a directional move
int maxThreshold = 1000;
int xValue = 0;
int yValue = 0;
int currentSeg = 0;
byte swState = HIGH;
byte lastSwState = HIGH;

const int displayCount = 4;

int registers[regSize];

byte dpState = HIGH;
const int blinkPeriod = 400;
unsigned long blinkStartMillis;
const unsigned long swLongPressTime = 3000;
unsigned long swPressTime;
const int multiplexDelay = 5;
int byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};
int displayValues[] = {0, 0, 0, 0};

void blink() {  //function to blink an LED if the blink period has ended
  if (millis() - blinkStartMillis > blinkPeriod) {
    dpState = !dpState; 
    blinkStartMillis = millis();  
  }
}

void writeReg(int encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}

void incrementDisplay() {
  if (yValue > maxThreshold && joyMoved == false) {
    joyMoved = true;
    if (displayValues[currentSeg] != encodingsNumber - 1) {
      displayValues[currentSeg]++;
    }
  }
}

void decrementDisplay() {
  if (yValue < minThreshold && joyMoved == false) {
    joyMoved = true;
    if (displayValues[currentSeg] != 0) {
      displayValues[currentSeg]--;
    }
  }
}

void moveLeft() {
  if (xValue < minThreshold && joyMoved == false) {
    joyMoved = true;
    if (currentSeg != displayCount - 1) {
      currentSeg++;;
    }
  }
}

void moveRight() {
  if (xValue > maxThreshold && joyMoved == false) {
    joyMoved = true;
    if (currentSeg != 0) {
      currentSeg--;
    }
  }
}

void joyMoveBack() {
  if (yValue > minThreshold && yValue < maxThreshold && xValue > minThreshold && xValue < maxThreshold) {
    joyMoved = false;
  }
}

void activateDisplay(int digit) { //Lights up a display
  for(int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[digit], LOW);
}

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(pinSw, INPUT_PULLUP);
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  blinkStartMillis = millis();
}


void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSw);
  if (!isStateTwo) {
    moveLeft();
    moveRight(); 
    blink();
  }
  else {
    incrementDisplay();
    decrementDisplay();
  }
  //this sequence writes the whole display
  for (int i = 0; i < displayCount; i++) {
    if (i == currentSeg and dpState == HIGH) {
      writeReg(byteEncodings[displayValues[i]] + 1);//lighting the selected segment's dp
    }
    else {
      writeReg(byteEncodings[displayValues[i]]);
    }
    activateDisplay(i);
    delay(multiplexDelay); //multiplexing
  }
  joyMoveBack();
  //changes the current the state if the joystick button has been pressed
  if (swState != lastSwState) {   
    if (swState == LOW) {
      if (!isStateTwo) {
        //checks for a long press
        swPressTime = millis();
        isPressing = true;
      }
      isStateTwo = !isStateTwo;
      dpState = HIGH;
    }
    else {
      isPressing = false;
    }
  }
  //checks to see if the button has been pressed long enough for a reset
  if (isPressing) {
    if (millis() - swPressTime > swLongPressTime) {
      isStateTwo = false;
      for (int i = 0; i < displayCount; i++) {
        displayValues[i] = 0;
      }
      currentSeg = 0;
      isPressing = false;
    }
  }
  lastSwState = swState;
}
