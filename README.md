# Sort Of Space Invaders For The Arduino
Sort of space invaders for arduino.<br/><br/>
Display for this game consists of 2 16x2 character LCD displays.<br/>
Ideal display arrangement is placing the character LCDs one one above the other (in the 2d sense, not height).<br/>
The bottom LCD contains player character and top LCD contains enemy/ies.<br/>
Controlls consist of 1 potentiometer (controls player spaceship) and 1 button (bullet fire button).<br/>

Pinout is as follows:<br/>
- 23-27 top (enemy) LCD
- 24-31 bottom (player) LCD
- A10 potentiometer
- 18 button
- 40 beeper

Code is made to run on an Arduino Mega 2560 but with some changing of pin numbers it should work on Arduino Uno and the sorts.
