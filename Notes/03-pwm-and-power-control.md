# Embedded Systems Foundation — PWM and Power Control
Embedded Systems Foundation — Sections 101–130
 
## Purpose
 
This document records the learning progress from Section 101 through Section 130.
 
It distinguishes between:
 
- Concepts studied
- Firmware written or reviewed
- Hardware configurations discussed
- Behavior physically verified
- Items that still require implementation or testing

 
A concept discussed in a lesson must not automatically be treated as a physically tested result.
 
***
 
## Section 101–106 — PWM Fundamentals
 
### Core concepts
 
- Pulse Width Modulation (PWM)
- Duty cycle
- PWM frequency and period
- Relationship between duty cycle and average delivered power
- Practical PWM behavior on a microcontroller output

 
### Key relationships
 
- Period = 1 / Frequency
- Duty cycle (%) = ON time / Period × 100
- For an ideal digital PWM waveform, the average voltage is approximately supply voltage × duty-cycle ratio.

 
These relationships describe ideal behavior. Actual loads, drivers, and switching components may introduce additional effects.
 
***
 
## Section 107–109 — ATmega328P Timer1 Hardware PWM
 
### Configuration
 
The Arduino Uno uses an ATmega328P running at 16 MHz in the configuration used in these lessons.
 
Timer1 was configured for Fast PWM Mode 14.
 
- ICR1 defines TOP.
- OCR1A controls the compare value for channel A.
- OC1A is available on Arduino Uno digital pin D9.
- Prescaler: 8.

 
### Example firmware
 
```cpp
const uint8_t LED_PIN = 9;

void setup() {
    DDRB |= (1 << DDB1);

    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) |
             (1 << WGM12) |
             (1 << CS11);

    ICR1 = 3999;
    OCR1A = 2000;
}

void loop() {
}
```
 
### Expected timer behavior
 
With a 16 MHz clock, prescaler 8, and TOP of 3999:
 
- Timer clock = 2 MHz
- Timer counts per PWM period = 4000
- PWM frequency = 500 Hz
- OCR1A of 2000 produces approximately 50% duty cycle.

 
D9 is associated with PB1/OC1A on the ATmega328P.
 
### Important cautions
 
- Direct register configuration can conflict with Arduino functions that also depend on Timer1.
- The example assumes the standard 16 MHz Arduino Uno clock.
- Register values and timer mode must be checked against the target MCU datasheet.

 
***
 
## Section 110–117 — ADC-to-PWM Control
 
### Hardware arrangement
 
- Potentiometer outer terminal: 5 V
- Potentiometer other outer terminal: GND
- Potentiometer wiper: A0
- LED output: D9
- Current-limiting resistor: approximately 220–330 Ω in series with the LED

 
The LED circuit must have the resistor in series.
 
### Core concepts
 
- ATmega328P ADC resolution: 10 bits
- ADC output range: 0–1023
- Timer1 TOP in the example: 3999
- Mapping an ADC reading to a PWM compare value
- Integer arithmetic and numeric range
- Preventing intermediate multiplication overflow
- Sampling without unnecessary blocking delays

 
### Example firmware
 
```cpp
#include <Arduino.h>

const uint8_t POT_PIN = A0;

void setup() {
    DDRB |= (1 << DDB1);

    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) |
             (1 << WGM12) |
             (1 << CS11);

    ICR1 = 3999;
    OCR1A = 0;
}

void loop() {
    uint16_t adcValue = analogRead(POT_PIN);

    uint16_t pwmValue =
        ((uint32_t)adcValue * 3999) / 1023;

    OCR1A = pwmValue;
}
```
 
### Why the cast matters
 
The cast to `uint32_t` ensures that the multiplication is performed using a wider unsigned integer type before division. This prevents the intermediate product from overflowing a 16-bit integer.
 
### Expected behavior
 
Turning the potentiometer changes the ADC reading, which changes the PWM compare value and therefore the LED brightness.
 
The code above demonstrates the mapping logic. Hardware verification should be recorded separately from code review.
 
***
 
## Sections 118–125 — Integration and Intermediate Topics
 
The available course record does not contain enough reliable detail to reconstruct every individual lesson in Sections 118–125.
 
Do not invent missing lesson titles, code, experiments, or test results.
 
Before this range is considered fully documented, recover the original lesson notes or verify the exact topics covered.
 
For each recovered section, record:
 
1. Learning objective
2. Hardware and components used
3. Relevant circuit or register configuration
4. Firmware written
5. Expected behavior
6. Actual observed behavior
7. Debugging notes
8. Verification status

 
***
 
## Section 126 — MOSFET Fundamentals
 
### Core concepts
 
- MOSFET terminals: Gate, Drain, and Source
- Gate voltage controls the device's switching state
- Logic-level MOSFET selection
- Reading electrical characteristics in a datasheet
- Inductive loads and flyback protection

 
### Datasheet interpretation example: IRLZ44N
 
Values discussed in the lesson included:
 
- Maximum drain-source voltage: 55 V
- Maximum RDS(on): approximately 25 mΩ at VGS = 5 V
- Maximum RDS(on): approximately 35 mΩ at VGS = 4 V
- Gate-threshold voltage: approximately 1–2 V under the specified low test current
- Maximum gate-source voltage: ±16 V

 
These are example datasheet values, not universal MOSFET specifications. Consult the exact manufacturer's datasheet before building a circuit.
 
### Important distinction
 
Gate-threshold voltage does not mean the MOSFET is fully switched on. For power switching, evaluate RDS(on) at a gate voltage that the controller can actually provide.
 
### Verification status
 
The topic was studied conceptually. Do not mark a physical MOSFET circuit as tested unless it has been assembled and measured.
 
***
 
## Section 127 — Power Switching and Troubleshooting
 
### Example application
 
A 12 V, 0.5 A fan controlled by a microcontroller through an N-channel MOSFET.
 
### Low-side switching arrangement
 
- Supply +12 V → fan positive terminal
- Fan negative terminal → MOSFET Drain
- MOSFET Source → supply negative/GND
- Arduino D9 → 100–220 Ω series resistor → MOSFET Gate
- 10 kΩ resistor → between Gate and Source
- Arduino GND → supply negative/GND

 
The motor must not be powered directly from an Arduino GPIO pin.
 
### Flyback diode
 
For a conventional DC motor or inductive load:
 
- Diode cathode (striped end) → +12 V
- Diode anode → switched motor terminal / Drain

 
Select a diode with suitable current and reverse-voltage ratings for the load.
 
### Topics studied
 
- MOSFET conduction loss
- PWM switching behavior
- Gate-drive requirements
- Logic-level compatibility
- Component selection
- Common wiring faults

 
A first-order conduction-loss estimate is:
 
P = I² × RDS(on)
 
This estimate does not include switching losses or other circuit losses.
 
### Example behavior
 
The discussed example switches the fan ON for three seconds and OFF for three seconds.
 
The example is a design exercise. It is not evidence that a physical 12 V fan circuit was tested.
 
***
 
## Section 128 — Embedded System Architecture
 
### Functional chain
 
Sensor → Measurement Validation → Control Logic → Output Driver → Fan
 
### Hysteresis control
 
The example control policy uses two thresholds:
 
- Turn the fan ON at or above 30 °C.
- Turn the fan OFF at or below 27 °C.
- Between 27 °C and 30 °C, preserve the previous output state.

 
Hysteresis reduces rapid switching near a single threshold.
 
### Scheduling and fault handling
 
- Use `millis()` for periodic, non-blocking sampling.
- The example sampling interval is 500 ms.
- Validate sensor data before using it for control.
- Define an explicit response to invalid or unavailable measurements.

 
The appropriate fault response depends on the application and its safety requirements. Turning the fan ON was discussed as one possible policy, not a universal rule.
 
### Verification status
 
The architecture and control policy were studied. Actual temperature sensing requires a selected sensor, its wiring, and a validated reading implementation.
 
***
 
## Section 129 — Modular Firmware Design
 
### Intended separation of responsibilities
 
1. Sensor interface: acquire a measurement.
2. Validation: determine whether the measurement is usable.
3. Control logic: decide the required fan state.
4. Output driver: apply the state to the hardware.
5. Diagnostics: report faults or useful state information.
6. Scheduler: determine when periodic tasks run.

 
### Hysteresis function
 
The intended control function has the following interface:
 
```cpp
bool shouldFanBeOn(float temperature, bool currentState);
```
 
It should return the desired fan state according to the configured thresholds and the current state.
 
### Output function
 
```cpp
void applyFanOutput(bool turnOn);
```
 
This function represents the hardware-output boundary. Its implementation must match the actual driver circuit and pin configuration.
 
### Important design rule
 
Keep the control decision separate from the electrical switching implementation. This makes the control logic easier to test and the hardware driver easier to replace.
 
### Verification status
 
This section documents the intended modular structure. A placeholder sensor function must not be described as a completed sensor implementation.
 
***
 
## Section 130 — Integrated Project Review and Documentation
 
### Project concept
 
A temperature-controlled fan with:
 
- Periodic temperature sampling
- Measurement validation
- Hysteresis-based control
- A separate output-driver function
- Defined fault behavior
- Diagnostics and documentation

 
### Acceptance criteria
 
The design should demonstrate that:
 
1. The fan is requested ON at or above the ON threshold.
2. The fan is requested OFF at or below the OFF threshold.
3. The previous state is retained between the thresholds.
4. Invalid measurements are not treated as valid temperatures.
5. The chosen fault response matches the application's requirements.
6. Sampling does not depend on long blocking delays.
7. Sensor, control, and output responsibilities are separated.
8. Test results distinguish expected behavior from observed behavior.

 
### Project documentation checklist
 
- System overview
- Component and datasheet references
- Circuit diagram and pin assignments
- Firmware source
- Control thresholds and design rationale
- Test cases and recorded results
- Known limitations
- Future improvements

 
### Status distinction
 
Use these labels accurately:
 
- **Studied:** the concept was covered.
- **Designed:** a proposed solution or circuit was specified.
- **Implemented:** code or hardware was actually created.
- **Tested:** behavior was checked using a documented procedure.
- **Verified:** evidence supports the relevant acceptance criteria.

 
The complete physical temperature-controlled fan should not be described as verified until the required sensor, driver hardware, firmware, and test procedure have actually been implemented and checked.
 
***
 
## Overall Status Through Section 130
 
The course has progressed from MCU fundamentals and reusable button firmware into PWM, ADC mapping, MOSFET switching, modular firmware, and system-level design.
 
The documented code examples and design exercises are useful reference material, but they do not automatically establish that every associated hardware circuit has been physically tested.
 
Sections 118–125 remain incomplete in this record until their original topics are recovered and verified.
 
## Next Step — Section 131
 
The next phase should prioritize practical engineering work:
 
- A concrete circuit
- A clearly defined firmware task
- A measurement or debugging procedure
- Explicit acceptance criteria
- A record of what was actually tested

 
The goal is to demonstrate engineering competence through evidence, not simply to complete lessons.
