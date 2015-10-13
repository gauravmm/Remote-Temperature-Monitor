// Slider
//
// This uses an SRF05 distance sensor, plus some sound-based
// input to allow for linear scale input.

#ifndef SLIDER_h
#define SLIDER_h

#include <Arduino.h>

#define SRF_MEM 64
#define CLICK_MEM 10

void slider_begin(uint8_t pinSRFTrigger, uint8_t pinSRFEcho, uint8_t pinAudioInt);


#endif