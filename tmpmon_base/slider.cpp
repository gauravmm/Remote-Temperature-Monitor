#include "slider.h"

DataSmoothing slider_smooth = DataSmoothing();

void slider_begin(uint8_t pinSRFTrigger, uint8_t pinSRFEcho, uint8_t pinAudioInt) {
	srfhandler_begin(pinSRFTrigger, pinSRFEcho);
	audiohandler_begin(pinAudioInt);
}


uint8_t slider_get(uint8_t *slider_value) {
	uint8_t aud = audiohandler_get();
	uint16_t tmp;
	uint8_t srf = srfhandler_get(&tmp);

	if (tmp > SRF_SCALE_MAX) {
		tmp = SRF_SCALE_MAX + 1;
	}

	uint8_t tmp2 = slider_smooth.update(tmp & 0xFF);

	if (slider_value) {
		*slider_value = tmp2;
	}

	if (aud == AUDIOHANDLER_DBLCLICK) {
		return srf | SLIDER_CLICK;
	}
	return srf;
}

