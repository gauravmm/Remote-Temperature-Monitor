#include "srfhandler.h"

#include "config_base.h"

// 2^32 - 1
#define ULONG_MAX_VAL 4294967295L

uint8_t srf_pinT = 0;
uint8_t srf_pinE = 0;
volatile uint8_t srf_state = SRF_ATTENTION_LOST;
volatile uint8_t srf_attention_count = 0;
volatile uint16_t srf_distance;
volatile uint8_t srf_lose_focus = 0;
volatile unsigned long srf_timeRising = 0;
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

  uint8_t rv = srf_state;
  srf_state &= SRF_ATTENTION_HAVE;
  return rv;
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

    srf_distance = (uint16_t) (duration >> 6); // Scale it to a usable range.

    // Evolve the state.
    if(srf_lose_focus) {
    	// srf_lose_focus = 0;
    	srf_state = SRF_ATTENTION_LOST;
    	// srf_attention_count = 0;
    }

    // Is the signal in the attention zone?
    uint8_t in_attention = srf_distance >= SRF_SCALE_MIN && srf_distance <= SRF_SCALE_MAX;

    // If user is in attention region
    if (in_attention) {
      // Increase the attention count
      if(srf_attention_count < SRF_ATTENTION_TIME) {
        srf_attention_count++;

        // If we have enough attention_count, we acquire attention
        if (!(srf_state & SRF_ATTENTION_HAVE) && srf_attention_count == SRF_ATTENTION_TIME && !srf_lose_focus) {
          srf_state = SRF_ATTENTION_HAVE | SRF_ATTENTION_GOT;
        }
      }
    } else {
      // Decrease the attention count
      if(srf_attention_count > 0) {
        srf_attention_count--;

        // If we currently have attention but have lost it
        if (srf_attention_count == 0) {
          srf_lose_focus = 0;
          if (srf_state & SRF_ATTENTION_HAVE) {
            srf_state = SRF_ATTENTION_LOST;
          }
        }
      }
    }
  }
}

void srfhandler_lose_focus() {
	srf_lose_focus = 1;
}

void SRF_TIMER_ISR(void) {
  // The timer is triggered, so now we send a pulse to the SRF05 and
  // wait.

  digitalWrite(srf_pinT, HIGH);
  delayMicroseconds(10);
  digitalWrite(srf_pinT, LOW);
}
