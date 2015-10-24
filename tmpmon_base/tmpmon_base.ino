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

uint8_t x = 0;
void loop() {
  delay(50);

  uint8_t val = 0;
  uint8_t sl = slider_get(&val);
  // uint8_t sl = srfhandler_get(&val);
  
  Serial.print(sl);
  Serial.print('\t');
  Serial.print(val);
  Serial.println();

  if (sl & SLIDER_ACTIVE) {
    if (sl & SLIDER_CLICK) {
      slider_lose_focus();
      return;
    }

    display_roi(x, 0, 3, 8);


    if (val > 63) {
      val = 63;
    }
    val = 63 - val;
    
    for(uint8_t j = 0; j < 8; ++j) {
      uint16_t num = val - j*8;
      if (val < j*8)
        num = 0;
        
      num = (num > 8)?8:num;

      num = ((1 << num) - 1);
      num = (~num) & 0xFF;
      display_draw((uint8_t) num); 
    }
    for(uint8_t j = 0; j < 8; ++j) {
      display_draw(0); 
      display_draw(0);
    }

    x = (x + 1) % DISPLAY_WIDTH;
  }
}
