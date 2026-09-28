Embedded Systems Foundation — Core Concepts
 
 
1. Binary and Bits
 

 
A bit has two possible states:
 
0 1
 
An 8-bit value contains eight bits.
 
Example:
 
00001101 = 13
 
00110101 = 53
 

 
2. Bitwise Operations
 

 
SET a bit:
 
reg |= (1 << n);
 
CLEAR a bit:
 
reg &= ~(1 << n);
 
TEST a bit:
 
reg & (1 << n);
 
Assignment with = replaces the entire value.
 
Bitwise OR with |= modifies selected bits while preserving the others.
 

 
3. Pointers
 

 
A pointer stores an address.
 
Example:
 
int x = 10; int *p = &x;
 
x is the value.
 
&x is the address of x.
 
p stores the address of x.
 
*p accesses the value stored at that address.
 

 
4. References
 

 
A reference provides another name for an existing object.
 
Example:
 
void change(int &x) { x = 20; }
 
Changes made through the reference affect the original variable.
 

 
5. Memory
 

 
Typical embedded memory regions include:
 
Flash
 
Program storage.
 
RAM
 
Runtime data.
 
Stack
 
Local variables and function call data.
 
Heap
 
Dynamic memory allocated with malloc/new.
 
Embedded systems often avoid unnecessary dynamic allocation because fragmentation and non-deterministic allocation can be problematic.
 

 
6. volatile
 

 
volatile tells the compiler that a value may change outside the normal execution flow.
 
Typical embedded examples include:
 
Hardware registers
 
Variables modified inside interrupt handlers
 
volatile does not make an operation atomic and does not provide synchronization by itself.
 

 
7. Struct
 

 
A struct groups related data.
 
Example:
 
typedef struct { int temperature; int humidity; int pressure; } SensorData;
 

 
8. Pointer to Struct
 

 
A pointer can point to a struct.
 
Example:
 
SensorData *p = &sensor;
 
Member access:
 
p->temperature
 
is equivalent to:
 
(*p).temperature
 

 
9. Memory-Mapped I/O
 

 
Microcontrollers can map hardware registers into their address space.
 
Example:
 
unsigned char *reg = (unsigned char*)0x5000;
 
Writing through *reg can modify the hardware register located at that address.
 

 
10. Peripheral Registers
 

 
Registers control and report the state of hardware peripherals.
 
A peripheral can be represented as a struct:
 
typedef struct { volatile uint8_t CONTROL; volatile uint8_t STATUS; volatile uint8_t DATA; } Peripheral;
 

 
11. Bit Masks
 

 
Masks select specific bits or fields.
 
Example:
 
#define MODE_MASK 0x30
 
Reading a field:
 
mode = (CONTROL & MODE_MASK) >> 4;
 

 
12. GPIO
 

 
GPIO provides digital input and output.
 
Typical concepts:
 
Direction
 
Input
 
Output
 
Input register
 
Output register
 

 
13. Pull-Up
 

 
A pull-up resistor connects an input toward VCC.
 
With a button connected to GND:
 
Released → HIGH
 
Pressed → LOW
 
This is an Active-Low input.
 

 
14. Pull-Down
 

 
A pull-down resistor connects an input toward GND.
 
Typical behavior:
 
Released → LOW
 
Pressed → HIGH
 

 
15. Debouncing
 

 
Mechanical switches do not transition cleanly between states.
 
A button press can produce several rapid transitions.
 
Debouncing requires the signal to remain stable for a defined period before accepting the new state.
 

 
16. Timer
 

 
A timer counts clock ticks independently of normal delay-based software timing.
 
Example:
 
MCU clock = 16 MHz
 
Prescaler = 16
 
Timer clock = 1 MHz
 
Therefore:
 
1 tick = 1 microsecond
 

 
17. Interrupt
 

 
An interrupt temporarily transfers execution to an Interrupt Service Routine.
 
Example:
 
volatile uint32_t ticks = 0;
 
void ISR() { ticks++; }
 
The volatile qualifier is important because the variable is modified by the interrupt handler.
 

 
18. Non-Blocking Timing
 

 
Instead of stopping the CPU with delay(), firmware can compare timestamps.
 
Example:
 
if (ticks - previous >= interval) { previous = ticks;
 
```
// Execute periodic task
```
 
}
 
This allows other firmware tasks to continue running.
 

 
19. State
 

 
A state describes the current condition of a system.
 
For a button:
 
RELEASED
 
PRESS_DEBOUNCE
 
PRESSED
 
RELEASE_DEBOUNCE
 

 
20. Event
 

 
An event represents something that happened.
 
Example:
 
A button changing from released to pressed produces a press event.
 
State:
 
Button is currently pressed.
 
Event:
 
Button has just been pressed.
 

 
21. Raw State
 

 
Raw state is the direct hardware reading.
 
Example:
 
bool pressed = (digitalRead(button.pin) == LOW);
 

 
22. Stable State
 

 
Stable state is the firmware-approved state after debouncing.
 
Example:
 
button.stablePressed = true;
 
The stable state should only change after the debounce logic confirms the transition.
 

 
23. Button Driver
 

 
A Button Driver encapsulates the logic required to read, debounce, track, and generate events for a button.
 
The application should not need to know the internal debounce implementation.
 

 
24. Struct-Based Driver
 

 
A struct can hold the complete runtime state of a button.
 
Example:
 
struct Button { uint8_t pin; ButtonState state; unsigned long debounceStart; bool stablePressed; bool pressEvent; };
 

 
25. Array
 

 
An array stores multiple elements of the same type.
 
Example:
 
Button buttons[2];
 
The valid indexes are:
 
buttons[0]
 
buttons[1]
 
The number inside [] represents the number of elements when declaring the array.
 

 
26. Array-Based Driver
 

 
Multiple buttons can be updated using the same driver function.
 
Example:
 
for (uint8_t i = 0; i < BUTTON_COUNT; i++) { updateButton(buttons[i]); }
 
This removes duplicated button-handling code and makes the firmware easier to expand.
 

 
27. Firmware Architecture
 

 
The current architecture is:
 
Hardware Input
 
Raw State
 
Debounce / State Machine
 
Stable State
 
Event
 
Application Logic
 
Hardware Output
 
The Button Driver handles hardware-related state processing.
 
The Application decides what the detected event should do.
 
Current boundary:
 
Array-Based Button Driver
 
Next major topic:
 
PWM
