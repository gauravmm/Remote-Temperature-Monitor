#ifndef LED_INDICATOR_H
#define LED_INDICATOR_H

#include <Arduino.h>
#include "colormap.h"
#include "sensor_config.h"

void led_setup(void);

void led_set_value (uint8_t value);

void led_set_alarm (void);

#endif