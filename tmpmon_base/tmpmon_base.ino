 // Temperature Monitor
// CSCI 1600 Fall 2015
// Gaurav Manek <gmanek>

#include "config_base.h"
#include "slider.h"
#include "display.h"
#include "graphics.h"
#include "font_16x11.h"
#include "GraphData.h"

#define FOCUS_NONE     0
#define FOCUS_MENU     1
#define FOCUS_SET_TEMP 2

char menu_text [] = "Testing\0One\0Two\0Three";
char *menu_ptr [] = {&menu_text[0], &menu_text[8], &menu_text[12], &menu_text[16]};

uint8_t focus = FOCUS_NONE;

GraphData graphData;

// Celsius to Farenheit
float ctof(float c) {
  return 9*c/5 + 32;
}

//
// Code
//

void setup() {
  display_setup();
  slider_begin(PIN_SRF_TRIG, PIN_SRF_ECHO, PIN_AUDIO_INT);  
  
  Serial.begin(9600);
}

uint8_t x = 0;
uint8_t curr_focus = FOCUS_NONE;

void loop() {
  delay(20);
  
  // Receive Serial comms
  if(Serial.available()) {

  }

  // Process/Update the data
  graphData.push(x);
  x = (x + 1) % 32;

  // Update the user attention state

  // Draw graphics based on 
  if (focus == FOCUS_NONE) {
    graphics_print_main(graphData.render(), graphData.val(), graphData.vmin(), graphData.vmax(), GFX_UNIT_C);
  } else if (focus == FOCUS_MENU) {
    graphics_print_menu(menu_ptr, 1);
  } else if (focus == FOCUS_SET_TEMP) {

  } else {
    // Hey! You're not supposed to be here!
  }

  
  
  /*
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
  */
}
