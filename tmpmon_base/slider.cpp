#include "slider.h"

#include "srfhandler.h"
#include "audiohandler.h"

void slider_begin(uint8_t pinSRFTrigger, uint8_t pinSRFEcho, uint8_t pinAudioInt) {
	srfhandler_begin(pinSRFTrigger, pinSRFEcho);
	audiohandler_begin(pinAudioInt);
}
