# Temperature Monitor
Gaurav Manek

# Remote-Temperature-Monitor
CS1600 Final Project
Fall 2015, Brown University
Gaurav Manek <gmanek>

## Description

The project is to build a two-part device that can be used in a kitchen to monitor the surface temperature of food and candy as it is being cooked.

The measuring part of the device can be magnetically attached to the cooking hood, which uses an infrared thermocouple (the TI TMP007) to measure the temperature of the cooking surface. 

The separate base has a screen, a buzzer, and some no-touch user interaction. This base station will: 
 - display the current temperature,
 - show a graph of temperature over time (with automatic y-axis scaling), 
 - provide a temperature alarm that rings when the temperature goes above or below a user-settable threshold.
 - provide a user-interface designed to work with dirty/busy hands (i.e. without touching.)

These two parts communicate over the 868 MHz band using a generic Xbee clone. The device bodies will be 3d-printed.

## Progress

Broadly speaking, the breadboarding (for both the base and remote sensor) is mostly done, and the base station software is coming along quite nicely.

The next major course of action (after Tic-Tac-Toe is graded) is to build the user interaction.

### Hardware
 - [X] Procurement
   - [X] Arduino Nano V3.0
   - [X] Arduino Mega 2560
   - [X] `XRF` wireless RF radio (863 MHz band)
   - [X] `SSD1306` Screen
   - [X] TI `TMP007` Breakout Board
   - [X] `SRF05` ultrasonic distance sensor
   - [X] Microphone with op-amp signal booster
   - [X] Assorted bits and bobs
 - [ ] Breadboarding
     - [X] Base Station
     - [X] Arduino Mega 2560
     - [X] Screen
     - [X] Ultrasonic Sensor
     - [X] Microphone
     - [ ] Buzzer
     - [X] Wireless RF radio
   - [ ] Remote Sensor
     - [X] Arduino Nano V3.0
     - [X] `TMP007` Breakout Board
     - [X] Wireless RF radio
     - [X] Status LED
  
### Software

 - [X] Common
   - [X] Protocol design
 - [ ] Base Station
   - [ ] Screen
     - [X] Display library (Written for Tic-Tac-Toe)
     - [ ] Graphics
     - [X] 15x11 Font
   - [ ] User Interaction
     - [X] Distance scale
       - [X] Attention model ('recognize a user wants to issue commands')
       - [X] Interrupt-based sensor polling.
     - [X] Click sensor
       - [X] Interrupt-based double-clap detection.
     - [ ] Audio Output
     - [ ] Interface
 - [X] Remote Sensor
   - [X] DS18B20 Interfacing
   - [X] Communication 
   - [X] LED


## Changes from Proposal
 - `XRF` wireless radio instead of `nRF24L01`
 - More complex user interaction:
   - Instead of buttons, we use the `SRF05` ultrasonic distance sensor to allow for no-touch user interaction.
 - Upgrade from `ATTiny 85` to Arduino Nano V3.0
    - I need the stability & reliability of the hardware UART support.
 - Change from TI TMP007 IR thermocouple to Maxim DS18B20 1-Wire Digital Thermometer 
    - Recieved a faulty TMP007, and already had some DS18B20. Replacing the TMP007 would take too long (a few weeks)

## Font

The original screen font is based on font from:
https://github.com/gauravmm/HT1632-for-Arduino/tree/master/Arduino/HT1632

## Progress from Last Report

Since the last report, I have written the software for the remote sensor station, and changed the type of temperature sensor I'm using. The LED uses the Inferno color map from Matplotlib, chosen for perceptual linearity. (http://bids.github.io/colormap/)