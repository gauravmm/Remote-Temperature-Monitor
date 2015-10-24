#include "slider.h"

void slider_begin(uint8_t pinSRFTrigger, uint8_t pinSRFEcho, uint8_t pinAudioInt) {
	srfhandler_begin(pinSRFTrigger, pinSRFEcho);
	audiohandler_begin(pinAudioInt);
}


uint8_t slider_get(uint16_t *slider_value) {
	uint8_t aud = audiohandler_get();
	uint8_t srf = srfhandler_get(slider_value);

	if(aud == AUDIOHANDLER_DBLCLICK)
		return srf | SLIDER_CLICK;
}

