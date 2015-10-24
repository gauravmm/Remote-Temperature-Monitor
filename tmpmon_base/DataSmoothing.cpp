#include "DataSmoothing.h"

DataSmoothing::DataSmoothing() {
  for(uint8_t i = 0; i < SMOOTHING_PERIOD; ++i)
    data[i] = 0;
}

uint8_t DataSmoothing::update(uint8_t new_val) {
  // Remove the current value from the accumulator
  accum -= data[idx];
  // Set the new value in the array and add it to the accumulator
  data[idx] = new_val;
  accum += new_val;

  idx = (idx + 1) % SMOOTHING_PERIOD;
  return (uint8_t) (accum >> SMOOTHING_PERIOD_LOG);
}