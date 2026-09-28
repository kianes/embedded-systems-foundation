# Embedded Systems Foundation
 
A practical Embedded Systems learning journey focused on understanding both firmware and hardware from first principles.
 
This repository contains the completed learning path from C/C++ and microcontroller fundamentals through GPIO, registers, timers, interrupts, debouncing, reusable firmware architecture, Button Drivers, and Arrays.
 
The repository ends immediately before the PWM and Power Switching chapter.

## Study Notes
 
The complete study notes for this chapter are available here:
 
Read the complete Embedded Systems Study Notes
 
The notes cover the learning path from C/C++ fundamentals and MCU concepts through GPIO, timers, interrupts, debouncing, reusable Button Drivers, and Arrays.
 
## Project Documentation
 
Core concepts:
 
Core Concepts
 
Learning roadmap:
 
Roadmap
 
Button Driver architecture:
 
Button Driver Notes
 
Hardware setup:
 
Hardware Setup
 
Firmware:
 
Button Driver Array Firmware
 
## Hardware Platform
 
Arduino Uno
 
## Current Hardware Project
 
Two push buttons and one LED.
 
Button 1 → D7 Button 2 → D6 LED → D8 through a current-limiting resistor
 
Both buttons use the Arduino internal pull-up resistors.
 
## Topics Covered
 
C/C++ fundamentals
 
Pointers and references
 
Memory and memory-mapped I/O
 
Stack and Heap
 
volatile
 
Structs and typedef
 
Hardware registers
 
Bit manipulation and masks
 
GPIO
 
Pull-up and Pull-down
 
Active-Low signals
 
Debouncing
 
Timers
 
Interrupts
 
Non-blocking timing
 
Arduino GPIO abstraction
 
Button State Machines
 
Raw State and Stable State
 
Event Detection
 
Reusable Drivers
 
Struct-based Driver Architecture
 
References in Drivers
 
Multiple Button Management
 
Arrays
 
Firmware/Application separation
 
## Current Architecture
 
Hardware Input ↓ Raw State ↓ Debounce / State Machine ↓ Stable State ↓ Event ↓ Application Logic ↓ Hardware Output
 
## Next Chapter
 
PWM
 
MOSFET
 
Power Switching
 
After that, the learning path will move toward ADC, sensors, communication protocols, STM32, RTOS, PCB design, and larger embedded systems.
