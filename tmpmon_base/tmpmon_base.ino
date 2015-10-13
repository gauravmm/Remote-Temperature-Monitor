// Temperature Monitor
// CSCI 1600 Fall 2015
// Gaurav Manek <gmanek>

#include "config_base.h"
#include "srfhandler.h"
#include "audiohandler.h"


//
// Code
//

void setup() {
  srfhandler_begin(PIN_SRF_TRIG, PIN_SRF_ECHO);
  Serial.begin(9600);
}

void loop() {
  delay(100);
  uint16_t tmp = 0;
  uint8_t rv = srfhandler_get(&tmp);

  Serial.print(rv);
  Serial.print(' ');
  Serial.print(tmp);
  Serial.println();
}
