Embedded Systems Foundation — Hardware Setup
 
Platform
 
Arduino Uno
 
Button 1
 
Arduino Pin: D7
 
Connection:
 
D7 → Push Button → GND
 
The Arduino internal pull-up resistor is enabled in firmware using:
 
INPUT_PULLUP
 
Therefore:
 
Button released → HIGH
 
Button pressed → LOW
 
Button 1 is an Active-Low input.
 
Button 2
 
Arduino Pin: D6
 
Connection:
 
D6 → Push Button → GND
 
The Arduino internal pull-up resistor is enabled in firmware.
 
Therefore:
 
Button released → HIGH
 
Button pressed → LOW
 
Button 2 is an Active-Low input.
 
LED
 
Arduino Pin: D8
 
Connection:
 
D8 → Current-Limiting Resistor → LED → GND
 
A resistor in the approximate range of 220Ω–330Ω can be used.
 
Firmware Configuration
 
LED:
 
LED_PIN = 8
 
Buttons:
 
Button 1 = D7
 
Button 2 = D6
 
Button Count:
 
BUTTON_COUNT = 2
 
The firmware uses the Arduino internal pull-up resistors, so no external pull-up resistors are required for this setup.
 
Functional Behavior
 
Button 1 press:
 
LED ON
 
Button 2 press:
 
LED OFF
 
Both buttons are debounced in software.
 
The Button Driver generates a press event only after the button has remained stable for the debounce period.
 
Hardware → GPIO → Button Driver → Event → Application → LED
 
This hardware configuration was physically assembled and tested with the corresponding firmware.
