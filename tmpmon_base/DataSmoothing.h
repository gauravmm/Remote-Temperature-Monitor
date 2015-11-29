#ifndef DATA_SMOOTHING_H
#define DATA_SMOOTHING_H

#include <Arduino.h>

#define SMOOTHING_PERIOD      4
#define SMOOTHING_PERIOD_LOG  2

class DataSmoothing
{
  public:
    DataSmoothing();
    uint8_t update(uint8_t new_val);
  private:
    uint8_t data[SMOOTHING_PERIOD];
    uint8_t idx = 0;
    uint16_t accum = 0;
};

#endif