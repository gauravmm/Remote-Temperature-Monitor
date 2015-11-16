#ifndef COLORMAP_H
#define COLORMAP_H

#include <Arduino.h>

uint16_t LED_BRIGHTNESS(uint8_t phase);

uint8_t *LED_COLOR(uint8_t val);

extern uint8_t LED_COLOR_ALARM[3];

#endif
