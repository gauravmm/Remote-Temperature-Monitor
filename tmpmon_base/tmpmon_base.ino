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

#define MENU_SETALARM    0
#define MENU_CHANGEVIEW  1
#define MENU_TOGGLECF    2
#define MENU_BACK        3

const float srf_temp_scale = 250.0f/(SRF_SCALE_MAX - SRF_SCALE_MIN);
const float srf_menu_scale = 4.0f/(SRF_SCALE_MAX - SRF_SCALE_MIN);

char menu_text [] = "Set Alarm\0Change View\0Toggle C/F\0Back";
char *menu_ptr [] = {&menu_text[0], &menu_text[10], &menu_text[22], &menu_text[33]};

uint8_t focus = FOCUS_NONE;

// Temperature alarm config
uint8_t tempAlarmSet = 0;
uint8_t tempAlarmIncreasing = 0;
float tempAlarmValue = 0.0f;
uint8_t tempAlarmCountdown = 0;


// Temperature unit
uint8_t tempUnit = GFX_UNIT_C;

// Current menu item
uint8_t menuIdx = 0;
uint8_t viewType = 0;

GraphData graphData;

//
// Code
//

void setup() {
  pinMode(PIN_ALARM, OUTPUT);

  display_setup();
  slider_begin(PIN_SRF_TRIG, PIN_SRF_ECHO, PIN_AUDIO_INT);  
  
  setUnit(GFX_UNIT_C);

  Serial.begin(SERIAL_BAUD);
}

void setUnit(uint8_t unit) {
  if(unit == GFX_UNIT_C) {
    tempUnit = GFX_UNIT_C;
    graphData.alignment(10.0);
  } else {
    tempUnit = GFX_UNIT_F;
    graphData.alignment(18.0);
  }
}

// Parse integer part of positive floats, discard the rest.
uint8_t parseInp(float *val) {
  static uint16_t v = 0;
  static uint8_t flush = 0;

  while(!flush && Serial.available()) {
    char c = Serial.peek();
    if (c >= '0' && c <= '9') {
      v = v * 10 + (c - '0');
      Serial.read();
    } else {
      flush = 1;
      if(v) {
        if(val)
          *val = (float) v;
        v = 0;
        return 1;
      }
    }
  }

  // Flush
  if(flush) {
    while(Serial.available()) {
      char c = Serial.read();
      if(c == '\n'){
        flush = 0;
        v = 0;
        break;
      }
    }
  }
  
  return 0;
}

uint8_t x = 0;
uint8_t curr_focus = FOCUS_NONE;
uint8_t prev_serial_tmp = 0;

void loop() {
  delay(20);

  float temperature = x;
  
  // Receive Serial comms
  uint8_t rv = parseInp(&temperature);
  if(rv && temperature > 0) {
    graphData.push(temperature);
  }

  // Process/Update the data

  // Check alarm
  if(tempAlarmSet) {
    uint8_t tempTrigger = tempAlarmIncreasing?(temperature > tempAlarmValue):(temperature < tempAlarmValue);
    if (tempTrigger) {
      tempAlarmSet = 0;
      tempAlarmCountdown = ALARM_DURATION;
      Serial.println('A');
      prev_serial_tmp = 0; // Reset this so it updates the temperature automatically.
    }
  }

  // Update the alarm state if necessary
  if (tempAlarmCountdown) {
    tempAlarmCountdown--;
    if (tempAlarmCountdown) {
      digitalWrite(PIN_ALARM, (x & _BV(ALARM_PERIOD))?HIGH:LOW);
    } else {
      digitalWrite(PIN_ALARM, LOW);
    }
  } 

  if (!tempAlarmCountdown) {
    uint8_t serial_temp = graphData.val() * (254/250.0);
    if (serial_temp != prev_serial_tmp) {
      prev_serial_tmp = serial_temp;
      // Send LED instructions
      Serial.print('V');
      Serial.println(serial_temp);
    }
  }

  
  // Update the user attention state
  uint8_t slider_val;
  uint8_t slider_attn = slider_get(&slider_val);

  if (focus == FOCUS_NONE) {
    if (slider_attn & SLIDER_ATTENTION_GOT) {
      focus = FOCUS_MENU;
    }

  } else if (focus == FOCUS_SET_TEMP) {
    if (slider_attn & SLIDER_ACTIVE) {
      tempAlarmValue = (slider_val - SRF_SCALE_MIN) * srf_temp_scale;
    }

    if (slider_attn & SLIDER_CLICK) {
      tempAlarmSet = 1;
      tempAlarmIncreasing = (tempAlarmValue > graphData.val());
      focus = FOCUS_NONE;
    } else if (slider_attn & SLIDER_ATTENTION_LOST) {
      focus = FOCUS_NONE;
    } 

  } else if (focus == FOCUS_MENU) {
    if (slider_attn & SLIDER_ACTIVE) {
      menuIdx = srf_menu_scale * (slider_val - SRF_SCALE_MIN);
    }

    if (slider_attn & SLIDER_ATTENTION_LOST) {
      focus = FOCUS_NONE;
    } else if (slider_attn & SLIDER_CLICK) {
      // Process the menu items based on what is happening.
      switch (menuIdx) {
        case MENU_CHANGEVIEW:
          viewType = !viewType;
          focus = FOCUS_NONE;
          break;

        case MENU_TOGGLECF:
          if(tempUnit == GFX_UNIT_C) {
            setUnit(GFX_UNIT_F);
          } else {
            setUnit(GFX_UNIT_C);
          }
          focus = FOCUS_NONE;
          break;

        case MENU_SETALARM:
          tempAlarmSet = 0;
          focus = FOCUS_SET_TEMP;
          break;

        default:
        case MENU_BACK:
          focus = FOCUS_NONE;
          break;
      }
    }
  } else {
    // Hey! You're not supposed to be here!
  }


  // Draw graphics based on the current focus state.
  if (focus == FOCUS_NONE || focus == FOCUS_SET_TEMP) {
    // graphics_print_main(graphData.render(), graphData.val(), graphData.vmin(), graphData.vmax(), GFX_UNIT_C, 1, 30.0);
    uint8_t showAlarmSetting = (focus == FOCUS_SET_TEMP);
    uint8_t showAlarmIcon = 0;
    if (showAlarmSetting)
      showAlarmIcon = x & _BV(ALARM_SET_BLINK);

    if(viewType) {
      graphics_print_main(graphData.render(), graphData.val(), graphData.vmin(), graphData.vmax(), tempUnit, tempAlarmSet || showAlarmSetting, tempAlarmValue, showAlarmIcon || tempAlarmSet);
    } else {
      graphics_print_alt(graphData.val(), tempUnit, tempAlarmSet || showAlarmSetting, tempAlarmValue, showAlarmIcon || tempAlarmSet);
    }
  } else if (focus == FOCUS_MENU) {
    graphics_print_menu(menu_ptr, menuIdx);
  } else {
    // Hey! You're not supposed to be here!
  }

  x++; // = (x + 1) % 32;
}
