# Button Driver and Arrays — Sections 36–100
 
This document preserves the recovered lesson sequence for Sections 36–100 of the Embedded Systems Foundation course.
 
**Platform:** Arduino Uno / ATmega328P
 
**Practical hardware:**
 
- Button 1 → D7 → Button → GND
- Button 2 → D6 → Button → GND
- LED → D8 through a current-limiting resistor
- Both buttons use `INPUT_PULLUP`

 
**Important:** Section 36 was not recovered from the original lessons. It is intentionally left undocumented rather than reconstructed from assumptions.
 
***
 
## Section 36 — Source Not Recovered
 
The original lesson text for Section 36 was not included in the recovered notes. This section remains an explicit gap.
 
***
 
## Section 37 — GPIO
 
GPIO means General Purpose Input/Output. It is the microcontroller interface used to interact with external hardware, such as LEDs, buttons, digital sensors, relays, and digital signals.
 
A GPIO pin is configured as either an input or an output.
 
## Section 38 — GPIO Input and Output
 
An output allows the MCU to drive a pin HIGH or LOW. An input allows firmware to sample the electrical level at a pin.
 
Output path:
 
MCU → Pin → Hardware
 
Input path:
 
Hardware → Pin → MCU
 
An LED is commonly controlled through an output, while a button is commonly read through an input.
 
## Section 39 — GPIO Concepts: DIR, OUT, IN
 
In a generic register model:
 
- `DIR` configures the direction.
- `OUT` stores the requested output level.
- `IN` represents the sampled input level.

 
Conceptually:
 
- `DIR = 0`: Input
- `DIR = 1`: Output
- `OUT = 0`: Drive LOW
- `OUT = 1`: Drive HIGH

 
`OUT` and `IN` are not interchangeable. Actual register names and behavior depend on the MCU.
 
## Section 40 — GPIO Output Register Manipulation
 
For conceptual pin 3:
 
```c
GPIO_DIR |= (1u << 3);
GPIO_OUT |= (1u << 3);
GPIO_OUT &= ~(1u << 3);
```
 
These operations demonstrate setting and clearing bits using shifts and bitwise operations.
 
`GPIO_DIR` and `GPIO_OUT` are generic examples, not universal Arduino register names.
 
## Section 41 — GPIO Input
 
For a conceptual pin 3:
 
```c
GPIO_DIR &= ~(1u << 3);

if (GPIO_IN & (1u << 3)) {
    // Pin 3 reads HIGH
}
```
 
The mask isolates the bit of interest.
 
## Section 42 — Floating Input
 
An input without a defined electrical bias can float. Electrical noise may cause it to appear to switch unpredictably between HIGH and LOW.
 
A pull-up or pull-down resistor provides a defined default level.
 
## Section 43 — Pull-down
 
A pull-down resistor connects the input toward GND. The button connects the input to VCC when pressed.
 
- Released: LOW
- Pressed: HIGH

 
This is an active-high arrangement.
 
## Section 44 — Pull-up
 
A pull-up resistor connects the input toward VCC. The button connects the input to GND when pressed.
 
- Released: HIGH
- Pressed: LOW

 
This is an active-low arrangement.
 
## Section 45 — Internal Pull-up
 
On Arduino:
 
```cpp
pinMode(4, INPUT_PULLUP);
```
 
This configures D4 as an input and enables the internal pull-up.
 
Connect the button between D4 and GND.
 
- Released: `HIGH`
- Pressed: `LOW`

 
For this circuit, an external pull-up resistor is normally unnecessary.
 
## Section 46 — Active-high and Active-low
 
**Active-high** means HIGH represents the active condition.
 
**Active-low** means LOW represents the active condition.
 
With `INPUT_PULLUP`, a button is usually active-low. The electrical level alone does not determine the meaning; the circuit and interface convention do.
 
## Section 47 — Mechanical Button Bounce
 
Mechanical contacts can produce several rapid transitions when a button is pressed or released.
 
Firmware may observe a sequence such as:
 
`HIGH → LOW → HIGH → LOW`
 
before the signal settles.
 
This phenomenon is called switch bounce.
 
## Section 48 — Debouncing
 
Debouncing prevents unstable transitions from being accepted immediately.
 
A non-blocking strategy is:
 
1. Detect a possible transition.
2. Record the time.
3. Continue running the rest of the program.
4. Sample the input again after the debounce interval.
5. Accept the transition only if the expected level remains present.

 
The project uses 5 ms as a learning parameter. Real hardware may require measurement and adjustment.
 
## Section 49 — Why Not `delay()`?
 
`delay(5)` blocks normal loop execution for the waiting period.
 
Non-blocking timing allows firmware to perform other work while the debounce interval elapses.
 
## Section 50 — Timer
 
A hardware timer counts clock ticks independently of ordinary program flow.
 
Example:
 
- MCU clock: 16 MHz
- Prescaler: 16
- Timer clock: 1 MHz
- Tick duration: 1 µs

 
This assumes the selected timer supports that configuration.
 
Timers can support time measurement, periodic events, and PWM generation.
 
## Section 51 — Prescaler
 
A prescaler divides the timer's input clock.
 
`timer_clock = source_clock / prescaler`
 
Example:
 
20 MHz / 20 = 1 MHz
 
At that frequency, each tick lasts 1 µs. A duration of 5 ms corresponds to 5000 ticks.
 
Actual configuration also depends on timer width, mode, and clock source.
 
## Section 52 — Timer Overflow
 
An N-bit counter has `2^N` distinct count values, from zero through `2^N - 1`, before wrapping.
 
An 8-bit counter therefore has 256 states:
 
`0–255`
 
At a 2 µs tick, one complete 256-count cycle lasts:
 
`256 × 2 µs = 512 µs`
 
## Section 53 — Interrupt
 
An interrupt temporarily transfers CPU execution to an Interrupt Service Routine (ISR) in response to an event.
 
After the ISR finishes, execution normally returns to the interrupted flow.
 
Interrupt handlers should be kept appropriately short and safe.
 
## Section 54 — Timer and Interrupt
 
Conceptual example for an interrupt that occurs every 1 ms:
 
```c
volatile uint32_t ticks = 0;

void ISR(void)
{
    ticks++;
}
```
 
If the interrupt really occurs every 1 ms:
 
- 100 ticks = 100 ms
- 1000 ticks = 1 second

 
The interval depends on correct timer configuration.
 
## Section 55 — `volatile`
 
`volatile` tells the compiler that a value may change outside the ordinary visible flow, for example in an ISR or through a memory-mapped hardware register.
 
```c
volatile uint32_t ticks = 0;
```
 
`volatile` does not make an operation atomic, thread-safe, or synchronized.
 
Shared multi-byte values may need additional protection, depending on the MCU and execution context.
 
## Section 56 — Non-blocking Timing
 
If `ticks` increments every millisecond, a task can be scheduled approximately every 100 ms without blocking:
 
```c
uint32_t previous = 0;

if ((uint32_t)(ticks - previous) >= 100u) {
    previous = ticks;

    // Run the periodic task
}
```
 
Unsigned subtraction is commonly used to handle timer wraparound, provided the interval and type assumptions are valid.
 
## Section 57 — State Machine
 
A state machine explicitly represents the current condition of a system and defines its allowed transitions.
 
The Button Driver uses four states:
 
- `RELEASED`
- `PRESS_DEBOUNCE`
- `PRESSED`
- `RELEASE_DEBOUNCE`

 
## Section 58 — Timer versus State Machine
 
A timer answers:
 
“How much time has elapsed?”
 
A state machine answers:
 
“What condition is the system in, and which transitions are allowed?”
 
Combining them enables non-blocking debounce.
 
## Section 59 — `button_is_pressed`
 
An abstraction can translate an active-low electrical signal into a logical pressed/not-pressed value:
 
```c
bool button_is_pressed(void)
{
    return !(GPIO_IN & (1u << 4));
}
```
 
LOW maps to `true`, while HIGH maps to `false`.
 
`GPIO_IN` is conceptual pseudocode. The Arduino implementation uses `digitalRead()`.
 
## Section 60 — Four-state Debounce
 
```cpp
enum ButtonState {
    RELEASED,
    PRESS_DEBOUNCE,
    PRESSED,
    RELEASE_DEBOUNCE
};

ButtonState state = RELEASED;
uint32_t debounceStart;
```
 
The state and timestamp preserve the driver's progress between calls to the update function.
 
## Section 61 — `RELEASED`
 
For an active-low button:
 
- HIGH means released.
- LOW means pressed.

 
When LOW is detected, record the start time and move to `PRESS_DEBOUNCE`.
 
The press is not yet confirmed.
 
## Section 62 — `PRESS_DEBOUNCE`
 
After the debounce interval, sample the input again.
 
- If it is still LOW, accept the press and enter `PRESSED`.
- If it is HIGH, cancel the candidate press and return to `RELEASED`.

 
## Section 63 — Arduino Implementation Setup
 
Practical wiring:
 
- Button → D4 → GND
- LED → D13, the built-in LED on common Uno boards
- D4 configured as `INPUT_PULLUP`

 
Released reads HIGH; pressed reads LOW.
 
## Section 64 — `pinMode`
 
```cpp
pinMode(4, INPUT_PULLUP);
pinMode(13, OUTPUT);
```
 
The first line configures D4 as an input with the internal pull-up enabled.
 
The second configures D13 as an output.
 
## Section 65 — `digitalRead`
 
`digitalRead(4)` returns `HIGH` or `LOW`.
 
In this circuit, LOW means the button is pressed:
 
```cpp
if (digitalRead(4) == LOW) {
    // Button is pressed
}
```
 
## Section 66 — `==` versus `=`
 
`=` assigns a value.
 
`==` compares two values and evaluates to a Boolean result.
 
Confusing them changes program behavior.
 
## Section 67 — `millis()`
 
Arduino's `millis()` returns the number of milliseconds since the program started.
 
A non-blocking elapsed-time check can be written as:
 
```cpp
debounceStart = millis();

if (millis() - debounceStart >= 5u) {
    // The debounce interval has elapsed
}
```
 
Unsigned elapsed-time subtraction is the standard pattern for handling `millis()` wraparound.
 
## Section 68 — Variable Scope
 
A local variable declared inside `setup()` is not directly accessible inside `loop()`.
 
If both functions need a timestamp, it can be declared at file scope or passed through an appropriate object or function interface.
 
```cpp
unsigned long debounceStart;
```
 
## Section 69 — Start the State Machine
 
```cpp
enum ButtonState {
    RELEASED,
    PRESS_DEBOUNCE,
    PRESSED,
    RELEASE_DEBOUNCE
};

ButtonState state = RELEASED;
unsigned long debounceStart;

void setup()
{
    pinMode(4, INPUT_PULLUP);
    pinMode(13, OUTPUT);
}
```
 
## Section 70 — Implement `RELEASED`
 
```cpp
if (state == RELEASED) {
    if (digitalRead(4) == LOW) {
        debounceStart = millis();
        state = PRESS_DEBOUNCE;
    }
}
```
 
This detects a candidate press and starts timing. It does not yet report a confirmed press.
 
## Section 71 — Implement `PRESS_DEBOUNCE`
 
```cpp
else if (state == PRESS_DEBOUNCE) {
    if (millis() - debounceStart >= 5u) {
        if (digitalRead(4) == LOW) {
            state = PRESSED;
        } else {
            state = RELEASED;
        }
    }
}
```
 
The transition is accepted only if the button is still pressed after the interval.
 
## Section 72 — State `PRESSED`
 
While pressed, the active-low input reads LOW.
 
When HIGH is detected, begin release debounce:
 
```cpp
else if (state == PRESSED) {
    if (digitalRead(4) == HIGH) {
        debounceStart = millis();
        state = RELEASE_DEBOUNCE;
    }
}
```
 
## Section 73 — State-machine Checkpoint
 
The expected sequence is:
 
`RELEASED` → LOW detected → `PRESS_DEBOUNCE` → stable LOW → `PRESSED` → HIGH detected → `RELEASE_DEBOUNCE` → stable HIGH → `RELEASED`.
 
The next step is to complete the release-debounce path and test the full program.
 
## Section 74 — Updating Stable State
 
Raw state is read directly from GPIO. Stable state changes only after the debounce process confirms the candidate transition.
 
```cpp
if (pressed) {
    if (millis() - button.debounceStart >= 5u) {
        button.state = PRESSED;
        button.stablePressed = true;
    }
}
```
 
This is a conceptual fragment that belongs inside the correct state-machine branch. A single LOW sample must not immediately set `stablePressed`.
 
## Section 75 — Why Stable State Is Not Rewritten as a New Press Every Loop
 
`loop()` can run thousands of times while the button is held.
 
The transition from released to pressed should be confirmed once. Afterward, `stablePressed` remains true until a confirmed release.
 
Repeatedly observing the same state must not create repeated press events.
 
## Section 76 — Stable State versus Press Event
 
- `stablePressed` describes the current debounced condition.
- `pressEvent` signals that a new press was just confirmed.

 
Conceptually:
 
```cpp
// Before a confirmed press:
stablePressed = false;

// On the update that confirms a new press:
stablePressed = true;
pressEvent = true;

// On later updates while still held:
stablePressed = true;
pressEvent = false;
```
 
The event is a one-update notification; the stable state persists.
 
## Section 77 — Generate Events Inside the Driver
 
The application should not duplicate GPIO and debounce logic:
 
```cpp
if (digitalRead(7) == LOW) {
    // Application logic mixed with hardware details
}
```
 
Instead, it consumes the driver's event:
 
```cpp
if (button.pressEvent) {
    // Application response
}
```
 
This creates a cleaner interface between the application and the hardware-specific driver.
 
## Section 78 — Complete Button-driver Flow
 
Hardware → GPIO → Raw State → Debounce → State Machine → Stable State → Event Detection → Press Event → Application
 
After a candidate press is detected, the driver starts debounce. If the input remains LOW for the configured interval, it confirms the stable state and raises a press event.
 
The event is cleared on the next update.
 
## Section 79 — Why Separate Driver and Application?
 
For example:
 
```cpp
if (button.pressEvent) {
    ledState = !ledState;
}
```
 
This application logic does not need to know about GPIO, `INPUT_PULLUP`, contact bounce, the debounce interval, `millis()`, or the internal state machine.
 
The driver owns those details; the application decides what a press means.
 
## Section 80 — `struct` as the Complete State of a Button
 
```cpp
struct Button {
    uint8_t pin;
    ButtonState state;
    unsigned long debounceStart;
    bool stablePressed;
    bool pressEvent;
};

Button button1;
```
 
Each `Button` object holds the pin number and the runtime data needed to track its own state.
 
## Section 81 — Reference in the Button Driver
 
```cpp
void updateButton(Button &button)
```
 
The `&` in this C++ parameter declares a reference.
 
The function works with the original object rather than a separate copy. Updating `button.state` therefore changes the state retained by the caller.
 
## Section 82 — Why Passing a Copy Is a Problem
 
```cpp
void updateButton(Button button)
```
 
This form receives a copy. Changes made to that local copy do not update the caller's object, so the state-machine progress would not persist across calls.
 
Passing by reference avoids that problem.
 
## Section 83 — First Step Toward Multiple Buttons
 
Two objects can use the same driver:
 
```cpp
Button button1;
Button button2;

updateButton(button1);
updateButton(button2);
```
 
This works, but repeating individual calls becomes cumbersome as the number of buttons grows.
 
## Section 84 — What Is an Array?
 
An array stores a fixed number of elements of the same type:
 
```cpp
uint8_t values[4];
```
 
The valid indexes are `0`, `1`, `2`, and `3`.
 
Indexing begins at zero, so the last index is the element count minus one.
 
## Section 85 — Array of Buttons
 
```cpp
Button buttons[2];
```
 
This creates two complete `Button` objects:
 
- `buttons[0]`
- `buttons[1]`

 
Each has its own pin, state, timestamp, stable state, and event flag.
 
## Section 86 — Initialize a Button Array
 
```cpp
Button buttons[2] = {
    {7, RELEASED, 0, false, false},
    {6, RELEASED, 0, false, false}
};
```
 
The first object uses D7 and the second uses D6.
 
The remaining fields initialize each state machine and event flag.
 
## Section 87 — Button Count
 
```cpp
const uint8_t BUTTON_COUNT = 2;
```
 
Using a named count makes the array size and processing loop easier to maintain:
 
```cpp
Button buttons[BUTTON_COUNT] = {
    {7, RELEASED, 0, false, false},
    {6, RELEASED, 0, false, false}
};
```
 
## Section 88 — Why Button Count Matters
 
When adding a third button, update the count and add the new initializer:
 
```cpp
const uint8_t BUTTON_COUNT = 3;

Button buttons[BUTTON_COUNT] = {
    {7, RELEASED, 0, false, false},
    {6, RELEASED, 0, false, false},
    {5, RELEASED, 0, false, false}
};
```
 
The generic update function and processing loop do not need a separate copy for each button.
 
## Section 89 — Loop Through the Array
 
```cpp
for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    updateButton(buttons[i]);
}
```
 
For two buttons, the loop processes index 0 and then index 1. It scales with the array count.
 
## Section 90 — Separate Responsibilities
 
`updateButton(Button &button)` updates one button.
 
The loop decides how many buttons to process.
 
Keeping these responsibilities separate makes the driver reusable.
 
## Section 91 — Initialize Hardware from the Array
 
```cpp
for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
    pinMode(buttons[i].pin, INPUT_PULLUP);
}
```
 
The hardware setup is derived from each object's configured pin, so adding a button does not require duplicating `pinMode()` statements.
 
## Section 92 — Array-based Driver Architecture
 
Button 1 (D7) + Button 2 (D6) → GPIO → Raw State → Button Driver → Debounce / State Machine → Stable State → Press Event → Application
 
Each button's runtime data is stored in its own array element.
 
## Section 93 — Application Logic
 
```cpp
if (buttons[0].pressEvent) {
    ledState = true;
}

if (buttons[1].pressEvent) {
    ledState = false;
}
```
 
The application responds to events rather than implementing debounce or reading the pins itself.
 
## Section 94 — Driver and Application Separation
 
The driver manages the hardware interface and reports a clean result.
 
The application decides what the result means.
 
The same architectural principle applies to GPIO, ADC, UART, and sensor drivers.
 
## Section 95 — Complete Array-based Button Driver
 
The following is the recovered teaching implementation. It is preserved as lesson history and has not been hardware-revalidated as part of this documentation update.
 
```cpp
#include <Arduino.h>

const uint8_t LED_PIN = 8;
const uint8_t BUTTON_COUNT = 2;

enum ButtonState
{
    RELEASED,
    PRESS_DEBOUNCE,
    PRESSED,
    RELEASE_DEBOUNCE
};

struct Button
{
    uint8_t pin;
    ButtonState state;
    unsigned long debounceStart;
    bool stablePressed;
    bool pressEvent;
};

Button buttons[BUTTON_COUNT] =
{
    {7, RELEASED, 0, false, false},
    {6, RELEASED, 0, false, false}
};

bool ledState = false;

void updateButton(Button &button)
{
    button.pressEvent = false;

    bool pressed = (digitalRead(button.pin) == LOW);

    if (button.state == RELEASED)
    {
        if (pressed)
        {
            button.state = PRESS_DEBOUNCE;
            button.debounceStart = millis();
        }
    }
    else if (button.state == PRESS_DEBOUNCE)
    {
        if (pressed)
        {
            if (millis() - button.debounceStart >= 5)
            {
                button.state = PRESSED;
                button.stablePressed = true;
                button.pressEvent = true;
            }
        }
        else
        {
            button.state = RELEASED;
        }
    }
    else if (button.state == PRESSED)
    {
        if (!pressed)
        {
            button.state = RELEASE_DEBOUNCE;
            button.debounceStart = millis();
        }
    }
    else if (button.state == RELEASE_DEBOUNCE)
    {
        if (!pressed)
        {
            if (millis() - button.debounceStart >= 5)
            {
                button.state = RELEASED;
                button.stablePressed = false;
            }
        }
        else
        {
            button.state = PRESSED;
        }
    }
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        pinMode(buttons[i].pin, INPUT_PULLUP);
    }
}

void loop()
{
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        updateButton(buttons[i]);
    }

    if (buttons[0].pressEvent)
    {
        ledState = true;
    }

    if (buttons[1].pressEvent)
    {
        ledState = false;
    }

    digitalWrite(LED_PIN, ledState);
}
```
 
## Section 96 — Practical Driver Test
 
The recovered practical result was:
 
- Button 1 on D7 turns the LED on.
- Button 2 on D6 turns the LED off.
- Holding a button does not continuously create new press events.
- Both buttons use the same driver function.

 
These are the recorded results from the lesson, not a new hardware test performed during documentation.
 
## Section 97 — Why This Milestone Matters
 
The project combines several reusable firmware patterns:
 
Object → Struct → Driver → Array → Loop → Application
 
These patterns can be applied to other peripherals and hardware interfaces.
 
## Section 98 — Extensibility
 
Increasing `BUTTON_COUNT` and adding another initializer lets the same processing loop handle additional buttons.
 
This is code reuse: one driver implementation serves multiple instances.
 
## Section 99 — From Simple Firmware to Structured Firmware
 
The project evolved from direct pin reads and output writes toward:
 
- Explicit state machines
- Non-blocking timing
- Structs and references
- Arrays
- Event generation
- Hardware abstraction
- Separation between driver and application

 
These concepts also apply when moving to more capable MCUs such as STM32.
 
## Section 100 — Button Driver Milestone
 
The course milestone was a reusable, array-based Button Driver tested on a physical Arduino Uno.
 
It separates button handling from application behavior and introduces multiple instances of one driver.
 
### Foundation checklist
 
- GPIO and input/output
- Pull-up / pull-down and active-low logic
- Bitwise operations, masks, and register concepts
- Pointers, references, structs, and arrays
- Timers, interrupts, `volatile`, and non-blocking timing
- State machines and debounce
- Raw state, stable state, and event detection
- Reusable driver and application separation
- Multiple buttons processed by one loop
- Physical Arduino implementation

 
The next course section begins with Section 101: PWM (Pulse Width Modulation), followed by power switching concepts and MOSFET-based load control.
