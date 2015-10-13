// Use interrupts to poll the SRF sensor continually, keeping an update
// of the latest distance and if we have "attention."

#ifndef SRFHANDLER_H
#define SRFHANDLER_H

#include <Arduino.h>

// States for the State Machine!
#define SRF_ATTENTION_NONE	0
#define SRF_ATTENTION_HAVE	1
#define SRF_ATTENTION_GOT 	2
#define SRF_ATTENTION_LOST 	4

void srfhandler_begin(uint8_t pinTrigger, uint8_t pinEcho);
uint8_t srfhandler_get(uint16_t *val);

#endif