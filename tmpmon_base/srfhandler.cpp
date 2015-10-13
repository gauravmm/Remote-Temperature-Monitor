#include "srfhandler.h"

// 2^32 - 1
#define ULONG_MAX_VAL 4294967295L

uint8_t srf_pinT = 0;
uint8_t srf_pinE = 0;
uint8_t srf_state;
uint16_t srf_distance;
unsigned long srf_timeRising = 0;
void SRF_ECHO_ISR();
void SRF_TIMER_ISR();

// Use our handler to handle the timer ISR.
ISR(TIMER3_OVF_vect)          
{
  if(srf_pinT)
    SRF_TIMER_ISR();
}

void srfhandler_begin(uint8_t pinTrigger, uint8_t pinEcho) {
  srf_pinT = pinTrigger;
  srf_pinE = pinEcho;

  pinMode(srf_pinT, OUTPUT);
  pinMode(srf_pinE, INPUT);

  attachInterrupt(digitalPinToInterrupt(srf_pinE), SRF_ECHO_ISR, CHANGE);

  // Set up the timer interrupt.
  // TCCR3A – Timer/Counter 3 Control Register A
  // TCCR3B - Timer/Counter 3 Control Register B

  cli(); // Disable interrupts

  TCCR3B = _BV(WGM33); // set PWM mode to "phase and frequency correct"
  TCCR3A = 0; // Clear Control Register A

  // From page 146, we calculate cycles by 
  // F_timer = F_CPU/(2 * prescaler * (1 + ICR3))
  // ICR3 = F_CPU/(2 * prescaler * F_timer) - 1
  // We want around 20Hz
  // This also sets the top value:
  ICR3 = F_CPU/(2 * 8 * 20) - 1;

  // Enable the timer interrupt
  TIMSK3 = _BV(TOIE3);

  // Set the prescaler to /8:
  TCCR3B |= _BV(CS31); 

  sei(); // Enable interrupts
}

uint8_t srfhandler_get(uint16_t *val) {
  if(val != 0) {
    *val = srf_distance;
  }

  return srf_state;
}

void SRF_ECHO_ISR(void) {
  if(digitalRead(srf_pinE)) {
    // Trigger when rising.
    srf_timeRising = micros();
  } else {
    // Trigger when falling.
    unsigned long now = micros();
    unsigned long duration;

    // Overflow handling.
    // we have to do this because it loops every 50 seconds.
    if (now < srf_timeRising) {
      duration = now + (ULONG_MAX_VAL - srf_timeRising);
    } else {
      duration = now - srf_timeRising;
    }

    srf_distance = (uint16_t) (duration >> 6); // From micros to millis.
    srf_state += 1;
    // Evolve the state.
  }
}

void SRF_TIMER_ISR(void) {
  // The timer is triggered, so now we send a pulse to the SRF05 and
  // wait.

  digitalWrite(srf_pinT, HIGH);
  delayMicroseconds(10);
  digitalWrite(srf_pinT, LOW);
}
