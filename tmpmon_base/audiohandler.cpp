#include "audiohandler.h"

// uint8_t ah_pin;
volatile uint8_t ah_state = AUDIOHANDLER_NONE;
volatile unsigned long ah_firstClickTime = 0;
void audiohandler_ISR(void);


void audiohandler_begin(uint8_t pinAudioInt) {
	// ah_pin = pinAudioInt;

	// Prepare pin for ISR
	pinMode(pinAudioInt, INPUT);
	attachInterrupt(digitalPinToInterrupt(pinAudioInt), audiohandler_ISR, RISING);
}

uint8_t audiohandler_get() {
	switch(ah_state) {
		case AUDIOHANDLER_DBLCLICK:
			ah_state = AUDIOHANDLER_NONE;
			return AUDIOHANDLER_DBLCLICK;
			break;
		case AUDIOHANDLER_CLICK:
		case AUDIOHANDLER_NONE:
			return ah_state;
			break;
	}
}

void audiohandler_ISR(void) {
	unsigned long now = millis();

	// Integer overflow. ABORT!
	if(now < ah_firstClickTime) {
		ah_firstClickTime = 0;
		ah_state = AUDIOHANDLER_NONE;
	}

	unsigned long dur = now - ah_firstClickTime;

	switch(ah_state) {
		case AUDIOHANDLER_CLICK:
			if(dur <= DOUBLECLICK_GAP_MIN) {
				// A click is underway. This falls into the first click, so we ignore it.
				break;
			} else if(dur < DOUBLECLICK_GAP_MAX) {
				// The second click has happened.
				ah_state = AUDIOHANDLER_DBLCLICK;
				break;
			} else {
				// Its outside the second click. 
				// We timeout this click and handle it as though no click has happened.
			}

		case AUDIOHANDLER_NONE:
			ah_firstClickTime = now;
			ah_state = AUDIOHANDLER_CLICK;
			break;

		case AUDIOHANDLER_DBLCLICK:
			// Ignore it.
			break;
	}
}