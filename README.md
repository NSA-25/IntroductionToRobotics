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

### Homework 3: [Controlling a 7-segment display](/Lab4)
In this project a joystick is used to control the position of a blinking segment and traverse the whole segment through neighbours (during state one)
and to decide whether or not a segment is turned on or off (during state two).

System's states:
State One: This is the default state and can be initiated after a button press in state two. The joystick can be used to travel across the display, the
current position being the segment that blinks. A short button press will toggle state two, while a long press will restart the display (turning all
segments off and changing the current position to decimal point).

State Two: Initiated after a button press in state one. The current segment will stop blinking and will be on or off (depending on its previous state).
The user can turn the current segment on or off using the joystick (going up to turn it on or down to turn it off).

List of possible moves:

a: left-down to f; right-down to b; down to g

f: up-right to a; down-right to g; right to b; down to e

b: up-left to a; down-left to g; left to f; down to c

g: up-left to f; up-right to b; down-left to e; down-right to c; up to a; down to d;

e: up-right to g; down-right to d; right to c; up to f

c: up-left to g; down-left to d; left to e; up to b; right to dp

d: left-up to e; right-up to c; up to g

dp: left to c

### Homework 4: [Controlling a 4 7-segment display](/Lab5)

In this project a joystick is used to travel through a 4 7-segment display. Four displays can be selected using the x-axis of the joystick and after
selection (joystick button press) their values can be increased or decreased selectively from 0 to F (Segments hold hexa values).

System's states:
State One: This is the default state and can be initiated after a button press in state two. The joystick can be used to travel across the display, the
current position being the display that has its decimal point blinking. A short button press will toggle state two, while a long press will restart 
the display (turning all segments's values to 0 and changing the current position to the rightmost display).

State Two: Initiated after a button press in state one. The current display's decimal point will turn on completely.
The user can increase of decrease the value in the display by 1 (from 0 to F).

### Homework 5: [Basic menu with a minigame](/Menu)
