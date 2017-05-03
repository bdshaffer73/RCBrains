# RCBrains
Hijacking an RC car control system using an Arduino Uno, to promote autonomous driving.

## Overview
This project involves enabling an Arduino Uno to control the transmitter of an RC Car. The transmitter and car each have circuitry for creating, trasmitting, receiving, and interpreting signals following a fixed communication protocol. Rather than reverse engineer and rebuild the entire transmission or reception system, we chose to put the current embedded system to work for us. 

The transmitter has 4 button-like pieces (metal tabs shown _here_) that tell the IC which way each joystick is being pushed. The controller can only transmit Left, Right, Neither, Forward, Backward, Stop. Neither and Stop are simply the lack of a signal from the each respective joystick. Our system reads the state of the joysticks, then imitates joystick actions. Our system is unable to prevent joystick input from being read by the controller because it is in parallel with the joysticks, rather than in series. This leaves possible edge cases such as Forward and Backward being pressed at the same time.

## Our strategy
To read and write to the controller, we simply soldered wires to the positive end of these buttons and to the system's common ground. When a button is not pressed, the voltage difference between the button and ground is approximately 4V. When the button is pressed, this drops to 0V. The current through this connection is approximately 40 micro-Amps. To emulate button presses, we controlled N-MOS transistors (ZVN3306A) with the arduino. The voltages and current in this system are well within the range of the Arduino and transistors [1][2].

To record the instructions of the driver and play them back, we included a Micro-SD storage card and reader. The Arduino libraries include functions for reading and writing to SD cards.

The Arduino Uno is equipped with six Analog Input pins. These pins are capable of reading an analog signal and converting it to a digital approximation between 0 and 1023 (https://www.arduino.cc/en/Tutorial/AnalogInputPins). The Uno is also equipped with 14 Digital I/O pins. Some of these pins have special functionality and are reserved for specific tasks, but all of them are capable of 5V (https://www.arduino.cc/en/Tutorial/DigitalPins). The 5V capability of the pins is perfect for our transistors, which have a switching threshold of .5V to 2.8V. In our case, the switching voltage is 5V. The transistors have a maximum conduction current of 270mA. From these specifications, we know that the arduino and chosen transistors are perfectly capable of handling the voltages and current of the controller electronics.

## Construction
!!!Discuss the pin arrangements

## Code
!!!Discuss the state machine
