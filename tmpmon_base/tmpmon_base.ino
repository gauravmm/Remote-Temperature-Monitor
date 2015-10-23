// Temperature Monitor
// CSCI 1600 Fall 2015
// Gaurav Manek <gmanek>

#include "config_base.h"
#include "slider.h"
#include "display.h"
#include "ssd1306.h"



//
// Code
//

void setup() {
  display_setup();
  slider_begin(PIN_SRF_TRIG, PIN_SRF_ECHO, PIN_AUDIO_INT);  
  
  Serial.begin(9600);
}

void loop() {
  delay(33);

  uint16_t val = 0;
  uint8_t sl = slider_get(&val);
  
  Serial.print(sl);
  Serial.print('\t');
  Serial.print(val);
  Serial.println();

  if (sl & SLIDER_ACTIVE) {
    if (sl & SLIDER_CLICK) {
      //slider_lose_focus();
      //return;
    }

    display_roi_clear();
    if (val > 63) {
      val = 63;
    }

    for (uint16_t i = 0; i < DISPLAY_WIDTH; ++i) {
      for(uint8_t j = 0; j < 8; ++j) {
        uint8_t num = val - j*8;
        if (val < j*8)
          num = 0;
          
        num = (num > 7)?7:num;

        num = ((1 << num) - 1) | (1 << num);
        display_draw(num); 
      }
    }  
  }
}
