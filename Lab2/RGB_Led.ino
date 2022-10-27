const int pinRedLed = 11;
const int pinGreenLed = 10;
const int pinBlueLed = 9;
const int pinPotRed = A0;
const int pinPotGreen = A1;
const int pinPotBlue = A2;
const int maxPotValue = 1023;
const int maxLedValue = 255;

//A function that turns the value taken by a potentiometer to one usable by a LED.
int ledValueFromPot(int value) {
   return map(value, 0, maxPotValue, 0, maxLedValue);
}

void setup() {
  pinMode(pinPotRed, INPUT);
  pinMode(pinPotGreen, INPUT);
  pinMode(pinPotBlue, INPUT);
  pinMode(pinRedLed, OUTPUT);
  pinMode(pinGreenLed, OUTPUT);
  pinMode(pinBlueLed, OUTPUT);
  Serial.begin(9600);
}

//Potentiometers values are read, turned into LED-friendly values and then used by the RGB LED.
void loop() {
  int redLedValue = ledValueFromPot(analogRead(pinPotRed));
  int greenLedValue = ledValueFromPot(analogRead(pinPotGreen));
  int blueLedValue = ledValueFromPot(analogRead(pinPotBlue));
  Serial.println("R = " + String(redLedValue) + " G = " + String(greenLedValue) + " B = " + String(blueLedValue));
  analogWrite(pinRedLed, redLedValue);
  analogWrite(pinGreenLed, greenLedValue);
  analogWrite(pinBlueLed, blueLedValue);
}