Embedded Systems Foundation — Learning Notes
 
This document records the main concepts and engineering decisions covered during the Embedded Systems learning path.
 
 
1. C/C++ and Embedded Foundations
 

 
The first stage focused on understanding how software interacts with the physical resources of a microcontroller.
 
Important concepts included:
 
Variables
 
Data types
 
Pointers
 
References
 
Structs
 
Memory
 
Stack
 
Heap
 
volatile
 
The goal was not only to write C/C++ code, but to understand what the code means at the memory and hardware level.
 

 
2. Pointers
 

 
A pointer stores an address.
 
Example:
 
int x = 10; int *p = &x;
 
The important distinction is:
 
x
 
The value.
 
&x
 
The address.
 
p
 
The stored address.
 
*p
 
The value located at that address.
 
This distinction becomes critical when working with hardware registers.
 

 
3. References
 

 
A reference provides another name for an existing variable.
 
Example:
 
void change(int &x) { x = 20; }
 
A reference is useful when a function needs to operate directly on an existing object rather than on a copy.
 
This became important later when the Button Driver started receiving Button objects by reference.
 

 
4. Memory-Mapped Hardware
 

 
Microcontrollers expose many hardware peripherals through registers located at specific addresses.
 
Conceptually:
 
Software ↓ Memory Address ↓ Hardware Register ↓ Peripheral
 
Writing to a memory-mapped register can therefore change the physical behavior of the microcontroller.
 

 
5. Registers and Bit Manipulation
 

 
Hardware registers often contain several independent control bits.
 
Changing one bit should not accidentally destroy the others.
 
Therefore bitwise operations are fundamental in embedded programming.
 
SET:
 
reg |= (1 << n);
 
CLEAR:
 
reg &= ~(1 << n);
 
TEST:
 
reg & (1 << n);
 
Masks allow groups of bits to be handled as fields.
 

 
6. GPIO
 

 
GPIO is the basic interface between firmware and digital electrical signals.
 
A GPIO can generally operate as:
 
Input
 
Output
 
As an output, firmware controls the pin.
 
As an input, firmware reads the electrical state of the pin.
 

 
7. Pull-Up and Pull-Down
 

 
An input cannot safely be left floating.
 
A pull-up biases the input toward VCC.
 
A pull-down biases the input toward GND.
 
With an internal pull-up and a button connected to GND:
 
Released:
 
HIGH
 
Pressed:
 
LOW
 
Therefore the button is Active-Low.
 

 
8. Debouncing
 

 
Mechanical buttons physically bounce during switching.
 
The electrical signal can therefore change several times before reaching a stable state.
 
A firmware debounce algorithm waits for the signal to remain stable for a defined period before accepting the transition.
 

 
9. Timer Concepts
 

 
A timer counts clock ticks.
 
The timer frequency can be derived from the MCU clock and prescaler.
 
Example:
 
16 MHz / 16 = 1 MHz
 
Therefore:
 
1 tick = 1 microsecond
 
Timers allow firmware to measure time without blocking the processor with delay-based execution.
 

 
10. Interrupts
 

 
An interrupt allows hardware or another event to request immediate attention from the CPU.
 
The CPU temporarily enters an Interrupt Service Routine.
 
After the ISR finishes, normal execution continues.
 
Variables modified by an ISR commonly require volatile.
 

 
11. Non-Blocking Firmware
 

 
Blocking code stops normal execution while waiting.
 
Example:
 
delay(1000);
 
During that delay, the application cannot perform normal loop processing.
 
Non-blocking timing instead compares timestamps or timer counters.
 
Example:
 
if (ticks - previous >= interval) { previous = ticks;
 
```
// task
```
 
}
 
This architecture allows several tasks to coexist.
 

 
12. Button State Machine
 

 
A button was implemented as a finite state machine.
 
States:
 
RELEASED
 
PRESS_DEBOUNCE
 
PRESSED
 
RELEASE_DEBOUNCE
 
The state machine separates:
 
Detection of a possible transition
 
Debounce timing
 
Confirmation of a stable transition
 
This is more reliable than treating every raw GPIO transition as a valid button event.
 

 
13. Raw State vs Stable State
 

 
Raw state comes directly from the GPIO.
 
Example:
 
bool pressed = (digitalRead(button.pin) == LOW);
 
Raw state can change rapidly because of mechanical bounce.
 
Stable state represents the state accepted by the firmware after debounce.
 
Therefore:
 
Raw State
 
is an electrical observation.
 
Stable State
 
is a firmware decision.
 

 
14. State vs Event
 

 
This distinction is fundamental.
 
State answers:
 
“What is true now?”
 
Event answers:
 
“What just happened?”
 
Example:
 
stablePressed = true
 
means the button is currently pressed.
 
pressEvent = true
 
means a new press has just been detected.
 
A held button should not continuously generate press events.
 

 
15. Button Driver
 

 
The button logic was moved into a reusable driver.
 
The driver is responsible for:
 
Reading the GPIO
 
Debouncing
 
Maintaining the state machine
 
Updating stable state
 
Generating press events
 
The application only needs to consume the resulting events.
 

 
16. Driver and Application Separation
 

 
The firmware was separated conceptually into two layers.
 
Driver:
 
Understands the hardware.
 
Application:
 
Understands what the hardware event should do.
 
Example:
 
Button Driver detects:
 
Button 1 was pressed.
 
Application decides:
 
Turn LED ON.
 
Button 2 was pressed.
 
Application decides:
 
Turn LED OFF.
 
This separation makes the code easier to reuse and expand.
 

 
17. Struct-Based Button Driver
 

 
A Button struct stores the complete runtime state of one button.
 
It contains:
 
Pin number
 
Current state
 
Debounce timestamp
 
Stable state
 
Press event
 
This means one object represents one complete button.
 

 
18. Passing the Driver by Reference
 

 
The update function receives the Button by reference:
 
void updateButton(Button &button)
 
This is important because updateButton() must modify the original Button object.
 
If the object were passed by value, the function would operate on a copy and the updated state would be lost after the function returned.
 

 
19. Multiple Buttons
 

 
Once one Button Driver worked correctly, the same driver could be used for multiple physical buttons.
 
Instead of creating separate functions:
 
updateButton1()
 
updateButton2()
 
the same function is used:
 
updateButton(button1);
 
updateButton(button2);
 
This removes duplicated logic.
 

 
20. Arrays
 

 
An array stores multiple objects of the same type.
 
Example:
 
Button buttons[2];
 
The objects are:
 
buttons[0]
 
buttons[1]
 
The first index is zero.
 
Therefore an array containing two elements has indexes 0 and 1.
 

 
21. Array-Based Button Driver
 

 
The Button objects were placed inside an array:
 
Button buttons[BUTTON_COUNT] = { {7, RELEASED, 0, false, false}, {6, RELEASED, 0, false, false} };
 
The driver can then update every button with a loop:
 
for (uint8_t i = 0; i < BUTTON_COUNT; i++) { updateButton(buttons[i]); }
 
This is the first significant step toward scalable firmware.
 
Adding another button mainly requires adding another Button object instead of writing another complete driver.
 

 
22. Current Firmware Architecture
 

 
The architecture developed so far is:
 
Hardware
 
↓
 
GPIO
 
↓
 
Raw State
 
↓
 
Debounce / State Machine
 
↓
 
Stable State
 
↓
 
Event
 
↓
 
Application Logic
 
↓
 
Output
 
This structure is more scalable than putting all hardware handling directly inside loop().
 

 
23. Current Practical Hardware
 

 
Arduino Uno
 
Button 1 → D7
 
Button 2 → D6
 
LED → D8
 
LED uses a current-limiting resistor.
 
Buttons are connected between their GPIO pins and GND.
 
The Arduino internal pull-up resistors are enabled.
 

 
24. Current Working Behavior
 

 
Button 1 generates a press event that turns the LED ON.
 
Button 2 generates a press event that turns the LED OFF.
 
The button handling is non-blocking and debounced.
 
Multiple buttons are handled using one reusable driver and an array.
 

 
25. End of Current Chapter
 

 
The current chapter ends with:
 
Reusable Button Driver
 
Struct
 
Reference
 
State Machine
 
Debouncing
 
Raw State
 
Stable State
 
Event Detection
 
Multiple Buttons
 
Arrays
 
The next chapter begins with a major transition from digital button input to hardware-controlled output power.
 
Next:
 
PWM
 
MOSFET
 
Power Switching
