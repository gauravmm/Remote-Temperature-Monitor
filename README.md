# Temperature Monitor
Gaurav Manek

# Remote-Temperature-Monitor
CS1600 Final Project
Fall 2015, Brown University
Gaurav Manek <gmanek>

## Description

The project is to build a two-part device that can be used in a kitchen to monitor the surface temperature of food and candy as it is being cooked.

The measuring part of the device can be hooked onto the side of a cooking vessel, and the sensors will measure the temperature of the cooking surface directly.

The separate base has a screen, a buzzer, and some no-touch user interaction. This base station will: 
 - display the current temperature,
 - show a graph of temperature over time (with automatic y-axis scaling), 
 - provide a temperature alarm that rings when the temperature goes above or below a user-settable threshold.
 - provide a user-interface designed to work with dirty/busy hands (i.e. without touching.)

These two parts communicate over the 868 MHz band using a generic Xbee clone. The device bodies will be 3d-printed.

## Attribution

I used the following libraries/code/etc. in this project:

  - Font design and rendering code from previous work by me. 
    - [https://github.com/gauravmm/HT1632-for-Arduino](HT1632 for Arduino library)
  - Matplotlib colormap 'Magma' by Nathaniel J. Smith and Stefan van der Walt
    - [https://github.com/BIDS/colormap/blob/master/colormaps.py](mpl colormaps)
    - Released under CC0 license
  - Arduino Library for Maxim Temperature Integrated Circuits by Miles Burton, Tim Newsome, et. al.
    - [https://github.com/milesburton/Arduino-Temperature-Control-Library](Arduino Temperature Control Library)
    - Released under the GNU Lesser General Public License 
  - OneWire Library by Paul Stoffregen, Jim Studt, et. al.
    - [www.pjrc.com/teensy/td_libs_OneWire.html](OneWire Library)
    - Released to public domain

The rest of the code is mine, and released under the MIT License.

## Changes from Proposal
 - `XRF` wireless radio instead of `nRF24L01`
 - More complex user interaction:
   - Instead of buttons, we use the `SRF05` ultrasonic distance sensor to allow for no-touch user interaction.
 - Upgrade from `ATTiny 85` to Arduino Nano V3.0
    - I need the stability & reliability of the hardware Serial support.
 - Change from TI TMP007 IR thermocouple to Maxim DS18B20 1-Wire Digital Thermometer 
    - Recieved a faulty TMP007, and already had some DS18B20. Replacing the TMP007 would take too long.
 - Added features
    - Two interfaces to choose from.
    - Font upscaling, with automatic smoothing.
    - Unit choice
    - Bidirectional alarm
