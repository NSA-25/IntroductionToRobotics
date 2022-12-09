#include <LedControl.h> // need the library
#include <LiquidCrystal.h>
#include <EEPROM.h>

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

const int brightLevel = 1;
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

byte arrowLeft[8] = {
	0b00010,
	0b00100,
	0b01000,
	0b10000,
	0b10000,
	0b01000,
	0b00100,
	0b00010
};

byte arrowRight[8] = {
	0b01000,
	0b00100,
	0b00010,
	0b00001,
	0b00001,
	0b00010,
	0b00100,
	0b01000
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

const int gameTime = 10000;
int score = 0;

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

int memoryNameStart = 101;
int memoryNameMulti = 10;
int memoryScore = 200;
int memoryWinnersSize = 0;

int foodPosX = rand() % 8;
int foodPosY = rand() % 8;

const int settingsPageMax = 2;

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
  EEPROM.update(brightLevel, 7);
  Serial.begin(9600);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, EEPROM.read(brightLevel)); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  matrix[xPos][yPos] = 1;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.createChar(0, arrowUp);
  lcd.createChar(1, arrowDown);
  lcd.createChar(2, arrowLeft);
  lcd.createChar(3, arrowRight);
  lc.setIntensity(0, 14);
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
        state = 1;
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
  lcd.print("Move");  
  lcd.setCursor(8, 1);
  lcd.write(byte(2));
  lcd.setCursor(10, 1);
  lcd.print("Play");
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
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Score: 0");
        state = 5;
        return;
      }
    }
  }
}

//brightness, setting page
void settings() {
  int settingsPage = 1;
  lcd.clear();
  while (true) {
    Serial.println(EEPROM.read(brightLevel));
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    joyMoveBack();
    if (!joyMoved) {
      if (moveDown()) {
        if (settingsPage == 0) {
          if (EEPROM.read(brightLevel) > 2) {
            EEPROM.update(brightLevel, EEPROM.read(brightLevel) - 1);    
            lc.setIntensity(0, EEPROM.read(brightLevel));     
          }
        }
        else if (settingsPage == 1) {
          state = 1;
          return;
        }    
      }
      else if (moveUp()) {
        if (settingsPage == 0) {
          if (EEPROM.read(brightLevel) < 14) {
            EEPROM.update(brightLevel, EEPROM.read(brightLevel) + 1);
            lc.setIntensity(0, EEPROM.read(brightLevel));        
          }
        }        
      }
      else if (moveLeft() && settingsPage != 0) {
        settingsPage--;
        lcd.clear();
      }
      else if (moveRight() && settingsPage < settingsPageMax-1) {
        settingsPage++;
        lcd.clear(); 
      }
    }
    if (settingsPage == 1) {
      lcd.setCursor(0, 0);
      lcd.write(byte(2));
      lcd.setCursor(4, 0);
      lcd.print("Settings ");
      lcd.write(byte(3));
    } 
    else if (settingsPage == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Brightness");
      lcd.print(" ");
      lcd.write(byte(0));
      lcd.print("up ");
      lcd.setCursor(0, 1);
      lcd.print(EEPROM.read(brightLevel));
      lcd.setCursor(9, 1);
      lcd.write(byte(1));
      lcd.print("down ");
      lcd.write(byte(3));
    }
  }
}
//100 winners number
//101-200 names
//201-210 scores
// void showWinner(n) {
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   string winner = "";
//   for (int i = memoryNameStart + (n-1)*memoryNameMulti; i < memoryNameStart + n*memoryNameMulti; i++) {
//     if (EEPROM.read(i)) {
//       winner.push_back(i);             
//     }
//     else {
//       break;
//     }
//   }
//   lcd.clear()
//   lcd.print(winner);
//   lcd.print(" ");
//   lcd.print(n);
//   lcd.setCursor(0, 1);
//   lcd.print("Score: ");
//   lcd.print(EEPROM.get(200 + n), DEC);
//   lcd.print(" ");
//   lcd.write(byte(2));
//   lcd.print(" ");
//   lcd.print(byte(3));
// }
void highscores() {
  int winnersSize = EEPROM.read(memoryWinnersSize);
  int winnerNum = 0;
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Highscores");
  lcd.setCursor(0, 1);
  lcd.write(byte(3));            
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Highscores");
  lcd.setCursor(0, 1);
  lcd.write(byte(3));
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
      // if (moveRight()) {
      //   if (winnerNum < 10) {
      //     delay(500);
      //     winnerNum++;
      //     if (winnerNum <= winnersSize) {
      //       showWinner(winnerNum);
      //     }
      //   }
      // }
      // if (moveLeft()) {
      //   if (winnerNum > 1) {
      //     delay(500);
      //     winnerNum--;
      //   } 
      //   else {
      //     lcd.clear();
      //     lcd.setCursor(3, 0);
      //     lcd.print("Highscores");
      //     lcd.setCursor(0, 1);
      //     lcd.write(byte(3));            
      //   }
      // }
    }
  }
}

void about() {
  int currentAbout = 0;
  lcd.clear();
  while (true) {
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    joyMoveBack();
    if (!joyMoved) {
      if (moveUp()) {
        state = 3;
        return;
      }
      if (moveLeft()) {
        lcd.clear();
        currentAbout = 1;
      }
      if (moveRight()) {
        lcd.clear();
        currentAbout = 0;
      }             
    }
    if (currentAbout == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Get the dots! ");
      lcd.write(byte(2));
      lcd.setCursor(0, 1);
      lcd.print("Only 10 seconds!");
    }
    if (currentAbout == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Neacsu Sasa ");
      lcd.write(byte(3));      
      lcd.setCursor(0, 1);
      lcd.print("Github: NSA-25");
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
      score++;
      lcd.setCursor(12, 0);      
      lcd.print(score, DEC);
      generateFood();
    }
  }
}

void startGame() {
  unsigned long long gameStart = millis();  
  while (gameStart + gameTime > millis()) 
  { 
    lcd.setCursor(5, 1);
    lcd.print("Time: ");
    lcd.setCursor(12, 1);
    lcd.print((int)(gameStart + gameTime - millis())/ 1000);
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
  lc.setLed(0, foodPosX, foodPosY, 0);
  lc.setLed(0, xPos, yPos, 0);
  state = 6;
  return;
}

void endGame(){
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Congratulations!");
  lcd.setCursor(1, 1);
  lcd.print("Score: ");
  lcd.print(score, DEC);
  delay(3000);
  // if (EEPROM.read(memoryWinnersSize) < 10) {
  //   int initPlace = EEPROM.read(memoryWinnersSize);
  //   int place = initPlace;
  //   while (score > EEPROM.READ(place-1 + memoryScore) {
  //     place--;
  //   }
        
  // } 
  score = 0;
  state = 1;
  return;
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
    Serial.println(EEPROM.read(brightLevel));
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
    case 6:
      endGame();
      break;
  }
}