# Embedded Systems Foundation — Roadmap
 
## Phase 1 — C/C++ and MCU Fundamentals
 
C/C++ fundamentals, pointers, references, memory concepts, stack and heap, volatile, structs, typedef, and pointers to structs.
 
## Phase 2 — Hardware Registers
 
Memory-mapped I/O, peripheral registers, register structures, bit manipulation, bit masks, bit fields, and hexadecimal representation.
 
## Phase 3 — GPIO
 
GPIO direction, digital input/output, pull-up, pull-down, and active-low signals.
 
## Phase 4 — Timing
 
Timers, prescalers, timer ticks, overflow, interrupts, and non-blocking timing.
 
## Phase 5 — Button Firmware
 
Mechanical button bounce, debouncing, button state machines, raw state, stable state, press events, and state versus event.
 
## Phase 6 — Reusable Firmware Architecture
 
Struct-based drivers, references, driver/application separation, multiple button support, arrays, and the array-based Button Driver.
 
Status: Completed through Section 100.
 
## Phase 7 — PWM Fundamentals
 
Sections 101–106.
 
PWM concepts, duty cycle, frequency, average power, and practical PWM behavior.
 
## Phase 8 — Timer1 Hardware PWM
 
Sections 107–109.
 
ATmega328P Timer1 configuration, Fast PWM Mode 14, prescaler selection, ICR1 as TOP, OCR1A as duty-cycle control, and hardware PWM output on Arduino Uno D9 (OC1A).
 
## Phase 9 — ADC-to-PWM Control
 
Sections 110–117.
 
Potentiometer input, 10-bit ADC readings, mapping ADC values to PWM duty cycle, integer arithmetic, numeric ranges, overflow prevention, and non-blocking sampling using millis().
 
## Phase 10 — Integrated Firmware and Hardware Concepts
 
Sections 118–125.
 
Continuation of PWM and embedded hardware/firmware integration. Detailed notes should reflect the actual lessons completed rather than assumed topics.
 
## Phase 11 — MOSFET Power Switching
 
Section 126.
 
MOSFET terminals, gate control, logic-level selection, datasheet interpretation, switching inductive loads, and flyback protection.
 
## Phase 12 — Power Switching Design and Troubleshooting
 
Section 127.
 
MOSFET conduction losses, PWM switching behavior, gate drive, logic-level compatibility, component selection, and troubleshooting a 12 V fan circuit.
 
## Phase 13 — Embedded System Architecture
 
Section 128.
 
Temperature-control architecture, hysteresis, non-blocking sampling, sensor validity, and safe-state behavior.
 
## Phase 14 — Modular Firmware Design
 
Section 129.
 
Separation of sensor reading, control logic, output handling, diagnostics, and scheduling.
 
## Phase 15 — Integrated Project and Documentation
 
Section 130.
 
Fan-controller design review, control-logic checks, error-handling requirements, project acceptance criteria, and documentation preparation.
 
## Current Status
 
Completed through Section 130.
 
The existing repository documents the foundational material through the array-based Button Driver. The PWM, ADC, MOSFET, and integrated-system lessons still need to be documented comprehensively.
 
The detailed record for Sections 101–130 must distinguish between concepts studied, code implemented, and hardware behavior physically verified.
 
## Next Step — Section 131
 
The next learning phase will emphasize practical engineering work: circuit design, firmware implementation, measurement, debugging, and systematic verification.
 
Each project should produce a concrete artifact and demonstrate understanding, not merely completion of a lesson.
