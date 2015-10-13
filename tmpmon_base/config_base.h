#include "config.h"

// 
// Pin Assignments
//

// Slider Control
#define PIN_SRF_ECHO 19
#define PIN_SRF_TRIG 11
#define PIN_AUDIO_INT 18

// OLED Screen
#define PIN_SSD1306_DC 		11
#define PIN_SSD1306_CS 		12
#define PIN_SSD1306_CLK 	10
#define PIN_SSD1306_MOSI 	 9
#define PIN_SSD1306_RESET 	13

// 
// User Interaction
// 

// Clicking
// Double-click settings:
#define DOUBLECLICK_GAP_MIN 100
#define DOUBLECLICK_GAP_MAX 700


// Distance
#define SRF_CM(x) 			((x)*58)
#define SRF_ATTENTION_TIME 	750
#define SRF_ATTENTION_MIN 	SRF_CM(30)
#define SRF_ATTENTION_MAX 	SRF_CM(60)
#define SRF_SCALE_MIN 		SRF_CM(20)
#define SRF_SCALE_MAX 		SRF_CM(80)