#include <LedControl.h> // need the library
#include <LiquidCrystal.h>
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int pinX = A0;
const int pinY = A1;
const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const byte pinSw = 13;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
// DIN, CLK, LOAD, No. DRIVER

byte arrowUp[8] = {
	0b00100,
	0b01110,
	0b11111,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110
};

byte arrowDown[8] = {
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b01110,
	0b11111,
	0b01110,
	0b00100
};

byte matrixBrightness = 2;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;

const int minThreshold = 384;
const int maxThreshold = 640;
const byte moveInterval = 100;
byte swState = HIGH;
byte lastSwState = HIGH;
bool joyMoved = false;
int xValue = 0;
int yValue = 0;
int swPressTime;
const int swLongPressTime = 1000;
bool isPressing = false;

const byte blinkInterval = 100;
int foodState = 0;

unsigned long long lastMoved = 0;
unsigned long long lastFoodBlink = 0;

const byte matrixSize = 8;
bool matrixChanged = true;

byte matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

byte matrixByte[matrixSize] = {
  B00000000,
  B01000100,
  B00101000,
  B00010000,
  B00010000,
  B00010000,
  B00000000,
  B00000000
};

int foodPosX = rand() % 8;
int foodPosY = rand() % 8;

void blinkFood() {
  lc.setLed(0, foodPosX, foodPosY, !foodState);
  foodState = !foodState;
}

void joyMoveBack() {
  if (yValue > minThreshold && yValue < maxThreshold && xValue > minThreshold && xValue < maxThreshold) {
    joyMoved = false;
  }
}

bool moveDown() {
  if (xValue < minThreshold && joyMoved == false) {
    joyMoved = true;
    return true;
  }
  return false;
}

bool moveUp() {
  if (xValue > maxThreshold && joyMoved == false) {
    joyMoved = true;
    return true;
  }
  return false;
}

bool moveLeft() {
  if (yValue < minThreshold && joyMoved == false) {
    joyMoved = true;
    return true;
  }
  return false;
}

bool moveRight() {
  if (yValue > maxThreshold && joyMoved == false) {
    joyMoved = true;
    return true;
  }
  return false;
}


void setup() {
  Serial.begin(9600);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  matrix[xPos][yPos] = 1;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.createChar(0, arrowUp);
  lcd.createChar(1, arrowDown);
}

int state = 0;
//0-start screen
//1-play screen
//2-settings
//3-about
//4-highscores
//5-the game
//6-end of the game

void startMenu() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Cool title!");
  delay(3000);
  lcd.setCursor(1, 1);
  lcd.print("Left to start");
  while (true) {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    joyMoveBack();
    if (!joyMoved) {
      if (moveLeft()) {
        state = 2;
        return;
      } 
    }
  }
}

void playScreen() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Press to play");
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.setCursor(3, 1);
  lcd.print("Move < Play");
  while (true) {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    joyMoveBack();
    if (!joyMoved) {
      if (moveUp()) {
        state = 2;
        return;
      }
      if (moveDown()) {
        state = 3;
        return;
      }
      if (moveLeft()) {
        state = 5;
        return;
      }
    }
  }
}

void settings() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Settings");
  while (true) {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    joyMoveBack();
    if (!joyMoved) {
      if (moveDown()) {
        state = 1;
        return;
      }
    }
  }
}

void highscores() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Highscores");
  while (true) {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    joyMoveBack();
    if (!joyMoved) {
      if (moveUp()) {
        state = 1;
        return;
      }
      if (moveDown()) {
        state = 4;
        return;
      }
    }
  }
}

void about() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Neacsu Sasa");
  lcd.setCursor(0, 1);
  lcd.print("Github: NSA-25");
  while (true) {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    joyMoveBack();
    if (!joyMoved) {
      if (moveUp()) {
        state = 3;
        return;
      }
    }
  }
}

// theoretical example
void generateFood() {
  int oldFoodPosX = foodPosX;
  int oldFoodPosY = foodPosY;
  while (oldFoodPosX == foodPosX || oldFoodPosY == foodPosY) {
    foodPosX = rand() % 8;
    foodPosY = rand() % 8;
  }
}

void updateByteMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixByte[row]);
  }
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void updatePositions() {
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);
  xLastPos = xPos;
  yLastPos = yPos;
  if (xValue < minThreshold) {
    if (xPos < matrixSize - 1) {
      xPos++;
    }
    else {
      xPos = 0;
    }
  }
  if (xValue > maxThreshold) {
    if (xPos > 0) {
      xPos--;
    }
    else {
      xPos = matrixSize - 1;
    }
  }
  if (yValue > maxThreshold) {
    if (yPos < matrixSize - 1) {
      yPos++;
    }
    else {
      yPos = 0;
    }
  }
  if (yValue < minThreshold) {
    if (yPos > 0) {
      yPos--;
    }
    else {
      yPos = matrixSize - 1;
    }
  }
  if (xPos != xLastPos || yPos != yLastPos) {
    matrixChanged = true;
    matrix[xLastPos][yLastPos] = 0;
    matrix[xPos][yPos] = 1;
    if (xPos == foodPosX && yPos == foodPosY) {
      generateFood();
    }
  }
}

void startGame() {
  if (millis() - lastMoved > moveInterval) {
    // game logic
    updatePositions();
    lastMoved = millis();
  }

  if (millis() - lastFoodBlink > blinkInterval) {
    blinkFood();
    lastFoodBlink = millis();
  }

  if (matrixChanged == true) {
    // matrix display logic
    updateMatrix();
    matrixChanged = false;
  }
}

void checkPress() {
  if (swState != lastSwState) {   
    swPressTime = millis();
    isPressing = true;
  }
  else {
    isPressing = false;
  }
}

void loop() {
  switch (state) {
    case 0:
      startMenu();
      break;
    case 1:
      playScreen();
      break;
    case 2:
      settings();
      break;
    case 3:
      highscores();
      break;
    case 4:
      about();
      break;
    case 5:
      startGame();
      break;
    // case 6:
    //   endGame();
    //   break;
  }
}