// Slider
//
// This uses an SRF05 distance sensor, plus some sound-based
// input to allow for linear scale input.

#ifndef SLIDER_H
#define SLIDER_H

#include <Arduino.h>
#include "srfhandler.h"
#include "audiohandler.h"

#define SLIDER_NONE 	SRF_ATTENTION_NONE
#define SLIDER_ACTIVE 	SRF_ATTENTION_HAVE
#define SLIDER_ATTENTION_GOT 	SRF_ATTENTION_GOT
#define SLIDER_ATTENTION_LOST 	SRF_ATTENTION_LOST
#define SLIDER_CLICK 	8

void slider_begin(uint8_t pinSRFTrigger, uint8_t pinSRFEcho, uint8_t pinAudioInt);

uint8_t slider_get(uint16_t *slider_value);

// Forcibly remove focus from the slider. 
#define slider_lose_focus() srfhandler_lose_focus()

#endif
