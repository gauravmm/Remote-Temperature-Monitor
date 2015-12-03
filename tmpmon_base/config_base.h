#include "config.h"

// 
// Pin Assignments
//

// Slider Control
#define PIN_SRF_ECHO        19
#define PIN_SRF_TRIG         8
#define PIN_AUDIO_INT       18
#define PIN_ALARM            5

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
#define DOUBLECLICK_GAP_MIN 300
#define DOUBLECLICK_GAP_MAX 1000


// Distance
// #define SRF_CM(x) 			((x)*58)
#define SRF_CM(x) 			(x)
#define SRF_ATTENTION_TIME 	(750/50)
#define SRF_ATTENTION_MIN 	SRF_CM(1)
#define SRF_ATTENTION_MAX 	SRF_CM(10)
#define SRF_SCALE_MIN 		SRF_CM(1)
#define SRF_SCALE_MAX 		SRF_CM(63)

// Alarm controls
#define ALARM_DURATION	    100
#define ALARM_PERIOD          2
#define ALARM_SET_BLINK       1

// Graphics Options
#define GFX_PADDING           4
#define GFX_PADDING_VERT      1
#define GFX_VERT_WD           2
#define GFX_UPSCALE_FACTOR_ALARM      3
#define GFX_UPSCALE_FACTOR_NOALARM    4
#define GFX_UPSCALE_SMOOTH    1

// LED lighting
#define LED_SCALE           (254/250.0)
