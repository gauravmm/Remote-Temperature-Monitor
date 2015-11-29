// Temperature Monitor
// CSCI 1600 Fall 2015
// Gaurav Manek <gmanek>

#include <OneWire.h>
#include <DallasTemperature.h>
#include "led_indicator.h"

#define TEMPERATURE_SEND_DELAY 1000

OneWire oneWire(PIN_TEMP);
DallasTemperature sensors(&oneWire);

#define BUF_SZ 8
char setinp [BUF_SZ];
uint8_t buf_i = 0;

unsigned long lastUpdate = 0;

void setup(void) {
  led_setup();
  Serial.begin(9600);
  sensors.begin();
}

uint8_t read_number_str(uint8_t* val, char *str, uint8_t max_len) {
  uint8_t i = 0;
  *val = 0;
  while (i < max_len) {
    if (*str >= '0' && *str <= '9') {
      *val = *val * 10 + (*str - '0');
    } else {
      break;
    }

    i++;
    str++;
  }
  return i;
}

void process_input() {
  // We receive Serial data, if any exists, and update the LED state
  if (buf_i == 1 && setinp[0] == 'A') {
    led_set_alarm();
  } else if (buf_i > 1 && setinp[0] == 'V') {
    // Process the Value assignment command:
    uint8_t i = 1;
    uint8_t v;
    uint8_t ln;

    ln = read_number_str(&v, setinp + i, buf_i - i);
    i += ln;

    if (i < buf_i - 1) {
      return; // Random garbage at the end. Ignore this.
    }

    led_set_value(v);
  }
}

/*
 * Main function, get and show the temperature
 */
void loop(void){
  delay(20);
  
  while(Serial.available()) {
    char n = Serial.read();
    if (n == '\n') {
      process_input();
      buf_i = 0;
    } else {
      if (n > 32 && n < 127) {
        if (n >= 'a' && n <= 'z') {
          n = n - 'a' + 'A';
        }
        // Prevent buffer overflow.
        if(buf_i >= BUF_SZ - 1) {
          buf_i = 0;
        }
        setinp[buf_i] = n;
        buf_i++;
      } else {
        // Ignore all non-printable characters and the space character.
      }
    }
  }

  unsigned long now = millis();
  if ((now < lastUpdate) || (lastUpdate + 500 < now)) {
    // call sensors.requestTemperatures() to issue a global temperature 
    // request to all devices on the bus
    sensors.requestTemperatures(); // Send the command to get temperatures
    // After we got the temperatures, we can print them here.
    // We use the function ByIndex, and as an example get the temperature from the first sensor only.
    float t = sensors.getTempCByIndex(0)/2 + sensors.getTempCByIndex(1)/2;
    Serial.print(t);  
    Serial.println();

    lastUpdate = now;
  }
}
