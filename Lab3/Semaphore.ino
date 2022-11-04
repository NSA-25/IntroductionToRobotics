const int pinButton = 2;
const int pinBuzzer = 13;
const int pinLedGreenPedestrian = 5;
const int pinLedRedPedestrian = 6;
const int pinLedGreenCar = 9;
const int pinLedYellowCar = 10;
const int pinLedRedCar = 11;

byte buttonState = 0;

int buzzerTone = 1000;
int timeStateOne = 8000;
int timeStateTwo = 3000;
int partOfTimeStateThree = 400;
int partOfTimeStateFour = 100;

void writeLeds(byte valueLedGreenPedestrian, byte valueLedRedPedestrian, byte valueLedGreenCar, byte valueLedYellowCar, byte valueLedRedCar) {
  //An useful function that decides which lights are on or off
  digitalWrite(pinLedGreenPedestrian, valueLedGreenPedestrian);
  digitalWrite(pinLedRedPedestrian, valueLedRedPedestrian);
  digitalWrite(pinLedGreenCar, valueLedGreenCar);
  digitalWrite(pinLedYellowCar, valueLedYellowCar);
  digitalWrite(pinLedRedCar, valueLedRedCar);
}

void stateOne() {
  //Red for pedestrians, green for cars
  writeLeds(0, 1, 1, 0, 0);
}

void stateTwo() {
  //Red for pedestrians, yellow for cars. The delay is the duration(3s)
  writeLeds(0, 1, 0, 1, 0);
  delay(timeStateTwo);
}

void stateThree() {
  //Green for pedestrians, red for cars. Lasts(8s)
  //Buzzer is activated at a constant interval
  writeLeds(1, 0, 0, 0, 1);
  for (int i = 0; i < 10; i++)
  {
      tone(pinBuzzer, buzzerTone);
      delay(partOfTimeStateThree);
      noTone(pinBuzzer);
      delay(partOfTimeStateThree);
  }
}

void stateFour() {
  //Blinking green for pedestrians, red for cars. Lasts(4s)
  //Buzzers's interval gets shorter
  for (int i = 0; i < 20; i++)
  {
    writeLeds(1, 0, 0, 0, 1);
    tone(pinBuzzer, buzzerTone);
    delay(partOfTimeStateFour);
    noTone(pinBuzzer);
    writeLeds(0, 0, 0, 0, 1);
    delay(partOfTimeStateFour);
  }
}

void setup() {
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(pinLedGreenPedestrian, OUTPUT);
  pinMode(pinLedRedPedestrian, OUTPUT);
  pinMode(pinLedGreenCar, OUTPUT);
  pinMode(pinLedYellowCar, OUTPUT);
  pinMode(pinLedRedCar, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //State 1 (green for cars, red for pedestrians) is active until the button is pressed.
  //8 seconds after the button is pressed a sequence of states happens and it ends in state 1
  buttonState = digitalRead(pinButton);
  stateOne();
  if (buttonState == LOW) {
    delay(timeStateOne);
    stateTwo();
    stateThree();
    stateFour();
  }
}
