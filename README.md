# IntroductionToRobotics
Homework assignments from the Introduction to Robotics laboratory (2022-2023)
# Small projects
### Homework 1: [Controlling an RGB LED](/Lab2)
In this project the brightness of each color of an RGB LED is controlled using potentiometers.

### Homework 2: [Simulating the traffic lights of a crosswalk](/Lab3)
In this project multiple states are used to simulate the traffic light of a crosswalk. There is one for the pedestrians (using a green and a red LEDs) 
and one for the cars (using a green, a yellow and a red LEDs).

State 1: default state, green for cars, red for people. Lasts indefinitely unless the button is pressed. No sounds.

State 2: appears 8 seconds after the button is pressed, yellow for cars, red for people. Lasts 3 seconds. No sounds.

State 3: red for cars, green for people and a beeping sound is used at a constant interval. Lasts 8 seconds.

State 4: red for cars, blinking green for people. The beeping sound is faster than in state 3. Lasts 4 seconds.

After state 4 it goes to state 1 and stays there until the button is pressed again.
LEDs' resistors have 2k and the buzzer has one with 5k.

