# RCBrains
Hijacking an RC car control system using an Arduino Uno, to promote autonomous driving.

## Overview
This project involves enabling an Arduino Uno to control the transmitter of an RC Car. The transmitter and car each have circuitry for creating, trasmitting, receiving, and interpreting signals following a fixed communication protocol. Rather than reverse engineer and rebuild the entire transmission or reception system, we chose to put the current embedded system to work for us. 

The transmitter has 4 button-like pieces (metal tabs shown _here_) that tell the IC which way each joystick is being pushed. The controller can only transmit Left, Right, Neither, Forward, Backward, Stop. Neither and Stop are simply the lack of a signal from the each respective joystick. Our system reads the state of the joysticks, then imitates joystick actions. Our system is unable to prevent joystick input from being read by the controller because it is in parallel with the joysticks, rather than in series. This leaves possible edge cases such as Forward and Backward being pressed at the same time.

This project was developed across Windows and Linux platforms. It should be noted that the Linux platform had unresolved issues installing the newest Arduino libraries, and was unable to use the digitPinToInterrupt() function.

## Our strategy
To read and write to the controller, we simply soldered wires to the positive end of these buttons and to the system's common ground. When a button is not pressed, the voltage difference between the button and ground is approximately 4V. When the button is pressed, this drops to 0V. The current through this connection is approximately 40 micro-Amps. To emulate button presses, we controlled N-MOS transistors (ZVN3306A) with the arduino. The voltages and current in this system are well within the range of the Arduino and transistors [1][2].

To record the instructions of the driver and play them back, we included a Micro-SD storage card and reader. The Arduino libraries include functions for reading and writing to SD cards.

The Arduino Uno is equipped with six Analog Input pins. These pins are capable of reading an analog signal and converting it to a digital approximation between 0 and 1023 (https://www.arduino.cc/en/Tutorial/AnalogInputPins). The Uno is also equipped with 14 Digital I/O pins. Some of these pins have special functionality and are reserved for specific tasks, but all of them are capable of 5V (https://www.arduino.cc/en/Tutorial/DigitalPins). The 5V capability of the pins is perfect for our transistors, which have a switching threshold of .5V to 2.8V. In our case, the switching voltage is 5V. The transistors have a maximum conduction current of 270mA. From these specifications, we know that the arduino and chosen transistors are perfectly capable of handling the voltages and current of the controller electronics.

## Pin connections

D2 - Button1_1

D3 - Button2_1

D4 - LeftTransistor_Gate

D5 - RightTransistor_Gate

D6 - UpTransistor_Gate

D7 - DownTransistor_Gate

D10 - CS (SD)

D11 - MOSI (SD)

D12 - MISO (SD)

D13 - SCK (SD)


A2 - LeftTransistor_Drain

A3 - RightTransistor_Drain

A4 - UpTransistor_Drain

A5 - DownTransistor_Drain


5V - Button1_2

5V - Button2_2

5V - VCC (SD)


GND - 10 kOhm - Button1_1

GND - 10 kOhm - Button2_1

GND - LeftTransistor_Source, RightTransistor_Source, UpTransistor_Source, DownTransistor_Source

GND - GND (SD)

GND - ControllerGND


ControllerLeft - LeftTransistor_Drain

ControllerRight - RightTransistor_Drain

ControllerUp - UpTransistor_Drain

ControllerDown - DownTransistor_Drain

## Code
Our state machine has three states: RUN, PLAY, and EC. RUN is the default state, and simply allows the driver to drive without recording or playing back instructions. REC records the driver's actions to the SD card, once per 10ms. PLAY reads the driver's actions from the SD card and writes them to the controller inputs once per 25ms. The time needed to record inputs is longer than the time needed to play them back. We introduced the delays to synchronize these two states. The time needed to perform the functions is neglible compared to the length of the delays.

Please refer to the Code section for more details. https://github.com/bdshaffer73/RCBrains/tree/master/RCBrains_SM

## Team work
Over the course of the project, Grant built most of the hardware and Ben built most of the code. For hardware, Ben soldered the connections to the controller and sourced the Arduino and MOSFETs. Grant did most of the wiring. For software, Grant did most of the debugging. Ben built the ISRs and state machine.

## Costs
The SD adapter was purchased for $5. The Arduino retails for $20. The RC Car is worth roughtly $20. The MOSFETs were worth $0.80 individually, but are cheaper in bulk. The SD card retails for $10-$30 depending on purchase location. We spent roughly 10 man hours working on the project.

Total price: $68 + labor

## Video demo
Here's a link to a video demoing the system. https://youtu.be/cCZ2MDA87To
