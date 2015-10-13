#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <Arduino.h>
#include "config_base.h"

// States for the State Machine!
#define AUDIOHANDLER_NONE 			0
#define AUDIOHANDLER_CLICK 			1
#define AUDIOHANDLER_DBLCLICK 		2

void audiohandler_begin(uint8_t pinAudioInt);
uint8_t audiohandler_get();

#endif
