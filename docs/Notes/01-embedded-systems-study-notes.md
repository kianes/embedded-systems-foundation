# Embedded Systems Foundation
 
## Study Notes
 
From C/C++ Fundamentals to Reusable Button Drivers
 
This document is the main study guide for the Embedded Systems Foundation chapter.
 
It covers the concepts learned before PWM and connects software concepts to real microcontroller hardware.
 
The practical platform used in this chapter is Arduino Uno.
 
***
 
# Part 1 — Embedded Systems Mindset
 
Embedded software is different from ordinary application software.
 
The program is directly connected to physical hardware.
 
A firmware engineer must therefore understand both:
 
Software
 
and
 
Hardware
 
The basic relationship is:
 
Software ↓ Microcontroller ↓ Peripheral ↓ Electrical Signal ↓ Physical Hardware
 
For example, when firmware changes a GPIO output:
 
Software command ↓ GPIO register ↓ Microcontroller pin ↓ Voltage changes ↓ LED turns ON or OFF
 
This relationship is the foundation of Embedded Systems.
 
***
 
# Part 2 — Binary and Bits
 
Microcontrollers work with binary information.
 
A bit can have only two states:
 
0
 
or
 
1
 
An 8-bit value contains eight bits.
 
Example:
 
00001101
 
This represents decimal 13.
 
Another example:
 
00110101
 
This represents decimal 53.
 
Each bit has a position.
 
For an 8-bit value:
 
Bit 7 Bit 6 Bit 5 Bit 4 Bit 3 Bit 2 Bit 1 Bit 0
 
Bit 0 is the least significant bit.
 
Bit 7 is the most significant bit.
 
***
 
# Part 3 — Bitwise Operations
 
Hardware registers often contain several independent control bits.
 
For example:
 
10010010
 
Changing one bit should not accidentally change the others.
 
This is why bitwise operations are fundamental in Embedded Systems.
 
To SET bit n:
 
reg |= (1 << n);
 
To CLEAR bit n:
 
reg &= ~(1 << n);
 
To TEST bit n:
 
reg & (1 << n);
 
The important difference between:
 
=
 
and
 
|=
 
is that = replaces the complete value, while |= modifies selected bits while preserving the others.
 
***
 
# Part 4 — Bit Masks
 
A mask selects specific bits.
 
Example:
 
#define MODE_MASK 0x30
 
0x30 in binary:
 
00110000
 
This mask selects bits 5 and 4.
 
To read the field:
 
mode = (CONTROL & MODE_MASK) >> 4;
 
The AND operation removes unrelated bits.
 
The shift moves the selected field into the correct position.
 
This technique is used constantly when working with MCU registers.
 
***
 
# Part 5 — Pointers
 
A pointer stores an address.
 
Example:
 
int x = 10;
 
int *p = &x;
 
Here:
 
x
 
is the value.
 
&x
 
is the address of x.
 
p
 
stores the address.
 
*p
 
accesses the value located at that address.
 
If we write:
 
*p = 25;
 
the value of x becomes 25.
 
This concept is critical because hardware registers can also be accessed through addresses.
 
***
 
# Part 6 — References
 
A reference is another name for an existing object.
 
Example:
 
void change(int &x) { x = 20; }
 
If we call:
 
int value = 10;
 
change(value);
 
value becomes 20.
 
References became important later when we created the Button Driver.
 
The driver must modify the original Button object.
 
Therefore:
 
void updateButton(Button &button)
 
receives the original object by reference.
 
***
 
# Part 7 — Memory
 
A microcontroller contains different types of memory.
 
Flash
 
Stores program code and other persistent data.
 
RAM
 
Stores runtime data.
 
Stack
 
Typically contains local variables and function call information.
 
Heap
 
Used for dynamic allocation such as malloc or new.
 
In Embedded Systems, dynamic allocation is often limited because fragmentation and non-deterministic allocation can make system behavior harder to predict.
 
***
 
# Part 8 — volatile
 
volatile tells the compiler that a variable may change outside the normal flow of the current code.
 
Typical examples include:
 
Hardware registers
 
Variables modified inside interrupt handlers
 
Example:
 
volatile uint32_t ticks = 0;
 
volatile does not mean:
 
atomic
 
thread-safe
 
synchronized
 
It only prevents the compiler from incorrectly assuming that the value cannot change unexpectedly.
 
***
 
# Part 9 — Struct
 
A struct groups related data.
 
Example:
 
typedef struct { int temperature; int humidity; int pressure; } SensorData;
 
Now SensorData represents one object containing several related values.
 
Structs become especially useful in Embedded Systems because a peripheral or driver often has multiple pieces of state that belong together.
 
***
 
# Part 10 — Pointer to Struct
 
A pointer can point to a struct.
 
Example:
 
SensorData *p = &sensor;
 
We can access members using:
 
p->temperature
 
This is equivalent to:
 
(*p).temperature
 
The arrow operator is therefore simply a convenient way to access a member through a pointer.
 
***
 
# Part 11 — Memory-Mapped I/O
 
Microcontrollers often expose hardware registers through memory addresses.
 
Conceptually:
 
Software ↓ Address ↓ Register ↓ Hardware Peripheral
 
For example:
 
unsigned char *reg = (unsigned char*)0x5000;
 
Here reg contains the address.
 
The expression:
 
*reg
 
represents the value stored at that address.
 
Writing to:
 
*reg
 
can therefore modify the corresponding hardware register.
 
This is the bridge between C/C++ and physical hardware.
 
***
 
# Part 12 — Peripheral Registers
 
A peripheral can contain several registers.
 
Conceptually:
 
CONTROL
 
STATUS
 
DATA
 
These registers can be represented using a struct:
 
typedef struct { volatile uint8_t CONTROL; volatile uint8_t STATUS; volatile uint8_t DATA; } Peripheral;
 
Then a pointer can represent the peripheral base address.
 
This allows firmware to interact with hardware using structured C code.
 
***
 
# Part 13 — GPIO
 
GPIO means General Purpose Input/Output.
 
A GPIO pin can normally operate as:
 
Input
 
or
 
Output
 
As an output, firmware controls the electrical state.
 
As an input, firmware reads the electrical state.
 
Conceptually:
 
Firmware ↓ GPIO configuration ↓ Pin ↓ Electrical signal
 
GPIO is one of the first places where software directly controls hardware.
 
***
 
# Part 14 — Pull-Up and Pull-Down
 
A digital input must not be left floating.
 
A floating input can randomly appear HIGH or LOW because of electrical noise.
 
A pull-up resistor biases the input toward VCC.
 
A pull-down resistor biases the input toward GND.
 
With a pull-up:
 
VCC ↓ Pull-Up ↓ GPIO ↓ Button ↓ GND
 
When the button is released:
 
GPIO = HIGH
 
When the button is pressed:
 
GPIO = LOW
 
This is called an Active-Low input.
 
Arduino provides an internal pull-up:
 
pinMode(pin, INPUT_PULLUP);
 
Therefore an external pull-up resistor is not required for our button circuit.
 
***
 
# Part 15 — Timer and Timing
 
A timer is a hardware counter that counts clock ticks.
 
Example:
 
MCU clock:
 
16 MHz
 
Prescaler:
 
16
 
Timer clock:
 
1 MHz
 
Therefore:
 
1 tick = 1 microsecond
 
Timers allow firmware to measure time without stopping the entire program.
 
This is fundamentally different from using:
 
delay()
 
because delay blocks normal execution.
 
***
 
# Part 16 — Interrupts
 
An interrupt allows an event to temporarily transfer CPU execution to an Interrupt Service Routine.
 
Example:
 
volatile uint32_t ticks = 0;
 
void ISR() { ticks++; }
 
Every time the interrupt occurs, ticks increases.
 
The variable is volatile because it is modified outside the normal execution flow.
 
Interrupts are important for:
 
Timers
 
Communication
 
External inputs
 
Peripheral events
 
Real-time systems
 
***
 
# Part 17 — Non-Blocking Timing
 
Blocking firmware waits.
 
For example:
 
delay(1000);
 
The processor spends that period waiting.
 
Non-blocking firmware checks whether enough time has passed.
 
Example:
 
if (ticks - previous >= interval) { previous = ticks;
 
```
// Execute task
```
 
}
 
This allows the rest of the firmware to continue operating.
 
This concept becomes extremely important when several tasks must run simultaneously.
 
***
 
# Part 18 — Mechanical Button Bounce
 
A physical button does not always produce one clean electrical transition.
 
When pressed, the contacts can rapidly switch:
 
HIGH LOW HIGH LOW HIGH LOW
 
before becoming stable.
 
If firmware reacts to every transition, one physical press may be interpreted as several presses.
 
This phenomenon is called switch bounce.
 
***
 
# Part 19 — Debouncing
 
Debouncing means filtering the unstable transition.
 
The basic strategy is:
 
Detect possible transition.
 
Record the time.
 
Wait for a defined period.
 
Check the input again.
 
Accept the transition only if the signal is still in the expected state.
 
For our project the debounce period was:
 
5 ms
 
The purpose is not to make the button physically cleaner.
 
The purpose is to prevent unstable electrical transitions from becoming firmware events.
 
***
 
# Part 20 — State Machine
 
A state machine represents the current condition of a system and defines valid transitions.
 
Our Button Driver uses four states:
 
RELEASED
 
PRESS_DEBOUNCE
 
PRESSED
 
RELEASE_DEBOUNCE
 
Normal press:
 
RELEASED ↓ PRESS_DEBOUNCE ↓ PRESSED
 
Normal release:
 
PRESSED ↓ RELEASE_DEBOUNCE ↓ RELEASED
 
This structure makes the debounce behavior explicit.
 
***
 
# Part 21 — Raw State
 
Raw state is the direct reading from the hardware.
 
Example:
 
bool pressed = (digitalRead(button.pin) == LOW);
 
Because our button uses INPUT_PULLUP:
 
LOW means pressed.
 
HIGH means released.
 
Raw state can still contain mechanical bounce.
 
Therefore it should not immediately control the application.
 
***
 
# Part 22 — Stable State
 
Stable state is the state accepted by the firmware after debounce.
 
Example:
 
button.stablePressed = true;
 
The important distinction is:
 
Raw State
 
What the electrical input says right now.
 
Stable State
 
What the firmware has accepted as valid.
 
This distinction is fundamental to reliable embedded firmware.
 
***
 
# Part 23 — State vs Event
 
State answers:
 
“What is true now?”
 
Event answers:
 
“What just happened?”
 
Example:
 
stablePressed = true
 
means:
 
The button is currently pressed.
 
pressEvent = true
 
means:
 
A new press has just been detected.
 
If the user holds the button down, stablePressed remains true.
 
But pressEvent should only become true once.
 
This distinction prevents one physical action from being repeatedly interpreted as many actions.
 
***
 
# Part 24 — Button Driver
 
Instead of putting all button logic directly inside loop(), we created a reusable Button Driver.
 
The driver handles:
 
GPIO reading
 
Debouncing
 
State transitions
 
Stable state
 
Press event generation
 
The application does not need to know how the debounce mechanism works.
 
This is the beginning of firmware architecture.
 
***
 
# Part 25 — Button Struct
 
Each physical button is represented by a Button object.
 
Example:
 
struct Button { uint8_t pin; ButtonState state; unsigned long debounceStart; bool stablePressed; bool pressEvent; };
 
Each field has a specific responsibility.
 
pin
 
Which GPIO belongs to this button.
 
state
 
Current state-machine state.
 
debounceStart
 
Time when debounce started.
 
stablePressed
 
Validated button state.
 
pressEvent
 
One-cycle press event.
 
The struct therefore contains the complete runtime state of one button.
 
***
 
# Part 26 — Passing Button by Reference
 
The driver function is:
 
void updateButton(Button &button)
 
The ampersand means the Button is passed by reference.
 
This is necessary because updateButton() modifies the Button.
 
For example:
 
button.state = PRESSED;
 
must modify the original object.
 
If the Button were passed by value, the function would modify only a copy.
 
The updated state would disappear when the function returned.
 
***
 
# Part 27 — Multiple Buttons
 
Once the driver worked with one button, we wanted to use the same logic for multiple buttons.
 
Instead of writing:
 
updateButton1()
 
and
 
updateButton2()
 
we use one reusable function:
 
updateButton(button1);
 
updateButton(button2);
 
This removes duplicated firmware logic.
 
***
 
# Part 28 — Array
 
An array stores multiple objects of the same type.
 
Example:
 
Button buttons[2];
 
This creates two Button objects.
 
Their indexes are:
 
buttons[0]
 
buttons[1]
 
The first index is zero.
 
Therefore:
 
BUTTON_COUNT = 2
 
means there are two elements.
 
***
 
# Part 29 — Array-Based Button Driver
 
The buttons are now stored in one array:
 
Button buttons[BUTTON_COUNT] = { {7, RELEASED, 0, false, false}, {6, RELEASED, 0, false, false} };
 
Button 1:
 
buttons[0]
 
Pin:
 
D7
 
Button 2:
 
buttons[1]
 
Pin:
 
D6
 
The same driver processes both:
 
for (uint8_t i = 0; i < BUTTON_COUNT; i++) { updateButton(buttons[i]); }
 
This is much more scalable than writing separate logic for every button.
 
***
 
# Part 30 — Application Logic
 
The driver generates events.
 
The application decides what those events mean.
 
Example:
 
if (buttons[0].pressEvent) { ledState = true; }
 
if (buttons[1].pressEvent) { ledState = false; }
 
The driver does not decide that Button 1 means LED ON.
 
That is application behavior.
 
This separation is important.
 
***
 
# Part 31 — Complete Firmware Architecture
 
The architecture we reached is:
 
Hardware Input
 
↓
 
GPIO
 
↓
 
Raw State
 
↓
 
Debounce
 
↓
 
State Machine
 
↓
 
Stable State
 
↓
 
Event
 
↓
 
Application Logic
 
↓
 
Hardware Output
 
This is the first step toward reusable embedded firmware.
 
***
 
# Part 32 — Practical Hardware
 
The project was physically assembled using:
 
Arduino Uno
 
Button 1 → D7
 
Button 2 → D6
 
LED → D8
 
LED → 220Ω–330Ω resistor → GND
 
Buttons:
 
D7 → Button → GND
 
D6 → Button → GND
 
Firmware:
 
INPUT_PULLUP
 
Therefore:
 
Released → HIGH
 
Pressed → LOW
 
***
 
# Part 33 — Final Working Behavior
 
The completed firmware behaves as follows:
 
Button 1 press
 
↓
 
Debounce
 
↓
 
Press Event
 
↓
 
LED ON
 
Button 2 press
 
↓
 
Debounce
 
↓
 
Press Event
 
↓
 
LED OFF
 
Holding a button does not continuously generate new press events.
 
Mechanical bounce is filtered by the Button Driver.
 
Both buttons are processed using one reusable driver.
 
The buttons are stored in an array.
 
***
 
# Part 34 — Why This Architecture Matters
 
At the beginning, the firmware was simply:
 
Read pin
 
↓
 
Control LED
 
That approach works for very small experiments.
 
But as the system grows, direct logic inside loop() becomes difficult to maintain.
 
The architecture developed in this chapter gives us:
 
Reusable drivers
 
Clear responsibilities
 
State management
 
Event generation
 
Scalability
 
Hardware abstraction
 
Application separation
 
These concepts will become even more important when we move to larger microcontrollers such as STM32.
 
***
 
# Part 35 — Chapter Boundary
 
Everything in this document belongs to the foundation chapter.
 
Completed:
 
C/C++
 
Pointers
 
References
 
Memory
 
Stack and Heap
 
volatile
 
Structs
 
Memory-Mapped I/O
 
Registers
 
Bitwise operations
 
Masks
 
GPIO
 
Pull-up
 
Pull-down
 
Active-Low
 
Timers
 
Interrupts
 
Non-blocking timing
 
Debouncing
 
State Machines
 
Raw State
 
Stable State
 
Events
 
Button Driver
 
References in Drivers
 
Multiple Buttons
 
Arrays
 
Application / Driver separation
 
The next chapter begins with a new hardware concept:
 
PWM
 
Pulse Width Modulation
 
PWM will allow firmware to control the average power delivered to hardware instead of simply switching a digital output ON or OFF.
 
This leads directly into:
 
PWM
 
MOSFET
 
Power Switching
 
and eventually real actuator and power-control circuits.
