//joystick pins
const int pinSW = 2; // digital pin connected to switch output
const int pinY = A0; // A1 - analog pin connected to Y output
const int pinX = A1; // A0 - analog pin connected to X output


// segment pins
const int pinA = 5;
const int pinB = 6;
const int pinC = 10;
const int pinD = 11;
const int pinE = 12;
const int pinF = 4;
const int pinG = 3;
const int pinDP = 9;
const int segSize = 8;

const int blinkPeriod = 200;
unsigned long blinkStartMillis;
unsigned long blinkCurrentMillis;

bool isStateTwo = false; //false means state 1, true means state 2

byte swState = HIGH;
byte lastSwState = HIGH;

int xValue = 0;
int yValue = 0;

//diagonal thresholds are closer to 512 (the normal joystick x and y values when unmoved)
//the reason there is quite a difference between normal and diagonal thresholds is to make
//diagonal movements (eg. g - f) easier
bool joyMoved = false;
int minThreshold = 222;
int diagonalMinThreshold = 448;
int maxThreshold = 800;
int diagonalMaxThreshold = 576;

bool isPressing = false;
const unsigned long swLongPressTime = 4000;
unsigned long swPressTime  = 0;
unsigned long swReleaseTime = 0;

bool segmentsState[segSize] = {false, false, false, false, false, false, false, false};

//directions
int up = 0;
int down = 1;
int left = 2;
int right = 3;
int upLeft = 4;
int upRight = 5;
int downLeft = 6;
int downRight = 7;

//pins
int pins[segSize] = {pinA, pinF, pinB, pinG, pinE, pinC, pinD, pinDP};
int pinAPosition = 0;
int pinFPosition = 1;
int pinBPosition = 2;
int pinGPosition = 3;
int pinEPosition = 4;
int pinCPosition = 5;
int pinDPosition = 6;
int pinDpPosition = 7;

int currentSeg = pinDpPosition;
int blinkLedPin = pinDP;
int prevPin = pinDP;

// this matrix stores the destination pin index for each segment 
// where it's possible to move in state 1
// if it's not possible to move in a certain direction it's gonne store a -1
// for that direction
// up  down  left  right  up-left up-right down-left down-right
int directionMatrix[segSize][segSize] = {
  {-1, pinGPosition, -1, -1, -1, -1, pinFPosition, pinBPosition}, // a
  {-1, pinEPosition, -1, pinBPosition, -1, pinAPosition, -1, pinGPosition}, // f
  {-1, pinCPosition, pinFPosition, -1, pinAPosition, -1, pinGPosition, -1}, // b
  {pinAPosition, pinDPosition, -1, -1, pinFPosition, pinBPosition, pinEPosition, pinCPosition}, // g
  {pinFPosition, -1, -1, pinCPosition, -1, pinGPosition, -1, pinDPosition}, // e
  {pinBPosition, -1, pinEPosition, pinDpPosition, pinGPosition, -1, pinDPosition, -1}, // c
  {pinGPosition, -1, -1, -1, pinEPosition, pinCPosition, -1, -1}, // d
  {-1, -1, pinCPosition, -1, -1, -1, -1, -1}, // dp
};

//movements:
// a: left-down to f; right-down to b; down to g
// f: up-right to a; down-right to g; right to b; down to e
// b: up-left to a; down-left to g; left to f; down to c
// g: up-left to f; up-right to b; down-left to e; down-right to c; up to a; down to d;
// e: up-right to g; down-right to d; right to c; up to f
// c: up-left to g; down-left to d; left to e; up to b; right to dp
// d: left-up to e; right-up to c; up to g
// dp: left to c

void moveUp() {
  if (yValue > maxThreshold && joyMoved == false && directionMatrix[currentSeg][up] != -1) {
    joyMoved = true;
    currentSeg = directionMatrix[currentSeg][up];
  }
}

void moveDown() {
  if (yValue < minThreshold && joyMoved == false && directionMatrix[currentSeg][down] != -1) {
    joyMoved = true;
    currentSeg = directionMatrix[currentSeg][down];
  }
}

void moveLeft() {
  if (xValue < minThreshold && joyMoved == false && directionMatrix[currentSeg][left] != -1) {
    joyMoved = true;
    currentSeg = directionMatrix[currentSeg][left];
  }
}

void moveRight() {
  if (xValue > maxThreshold && joyMoved == false && directionMatrix[currentSeg][right] != -1) {
    joyMoved = true;
    currentSeg = directionMatrix[currentSeg][right];
  }
}

void moveUpLeft() {
  if (yValue > diagonalMaxThreshold && xValue < diagonalMinThreshold && joyMoved == false && directionMatrix[currentSeg][upLeft] != -1) {
    joyMoved = true;
    currentSeg = directionMatrix[currentSeg][upLeft];
  }
}

void moveUpRight() {
  if (yValue > diagonalMaxThreshold && xValue > diagonalMaxThreshold && joyMoved == false && directionMatrix[currentSeg][upRight] != -1) {
    joyMoved = true;
    currentSeg = directionMatrix[currentSeg][upRight];
  }
}

void moveDownLeft() {
  if (yValue < diagonalMinThreshold && xValue < diagonalMinThreshold && joyMoved == false && directionMatrix[currentSeg][downLeft] != -1) {
    joyMoved = true;
    currentSeg = directionMatrix[currentSeg][downLeft];
  }
}

void moveDownRight() {
  if (yValue < diagonalMinThreshold && xValue > diagonalMaxThreshold && joyMoved == false && directionMatrix[currentSeg][downRight] != -1) {
    joyMoved = true;
    currentSeg = directionMatrix[currentSeg][downRight];
  }
}

void joyMoveBack() {
  if (yValue > diagonalMinThreshold && yValue < diagonalMaxThreshold && xValue > diagonalMinThreshold && xValue < diagonalMaxThreshold) {
    joyMoved = false;
  }
}

void blink() {  //function to blink an LED if the blink period has ended
  if (blinkCurrentMillis - blinkStartMillis > blinkPeriod) {
    digitalWrite(blinkLedPin, !digitalRead(blinkLedPin));  
    blinkStartMillis = blinkCurrentMillis;  
  }
}

void unBlink() {
 if (blinkLedPin != prevPin) {
   digitalWrite(prevPin, LOW);
 }
}

void turnOn() {
  if (yValue > maxThreshold && joyMoved == false) {
    joyMoved = true;
    segmentsState[currentSeg] = true;
    digitalWrite(pins[currentSeg], HIGH);
  }
}

void turnOff() {
  if (yValue < minThreshold && joyMoved == false) {
    joyMoved = true;
    segmentsState[currentSeg] = false;
    digitalWrite(pins[currentSeg], LOW);
  }
}

void turnAllOff() {
  for (int i = 0; i < segSize; i++) {
    segmentsState[i] = false;
  }
  currentSeg = 7;
  blinkLedPin = pinDP;
  prevPin = pinDP;
}

void displaySelected() {
  for (int i = 0; i < segSize; i++) {
    if (i != currentSeg) {
      digitalWrite(pins[i], segmentsState[i]);
    }
  }
}

void setup() {
  // initializing all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(pins[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  blinkStartMillis = millis();
}

void loop() {
	xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);
  blinkCurrentMillis = millis();

  //during state one the user is allowed to traverse the 7-segment display
  //in any possible direction (from to neighbour to neighbour)
  if (!isStateTwo) {
    moveUpLeft();
    moveUpRight();
    moveDownLeft();
    moveDownRight();
    moveUp();
    moveDown();
    moveLeft();
    moveRight();
    joyMoveBack();
    prevPin = blinkLedPin;
    blinkLedPin = pins[currentSeg];
    blink();
    unBlink();
  }
  //during state two the user is allowed to turn the selected segment on or off
  else {
    turnOn();
    turnOff();
    joyMoveBack();
  }

  //this code sequence checks if the joystick button has been pressed
  if (swState != lastSwState) {   
    if (swState == LOW) {
      if (!isStateTwo) {
        digitalWrite(pins[currentSeg], segmentsState[currentSeg]);
        swPressTime = millis();
        isPressing = true;
      }
      isStateTwo = !isStateTwo;
    }
    else {
      isPressing = false;
    }
  }

  //if the button has been pressed longer than 4 seconds it will turn off all segments
  if (isPressing) {
    if (millis() - swPressTime > swLongPressTime) {
      isStateTwo = false;
      turnAllOff();
    }
  }

  lastSwState = swState;
  displaySelected();
}                   