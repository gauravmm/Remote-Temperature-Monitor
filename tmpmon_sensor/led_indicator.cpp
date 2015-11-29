#include "led_indicator.h"

volatile uint8_t led_phase = 0;
volatile uint8_t led_phase_inc = 0;

volatile uint8_t led_alarm = 0;
volatile uint8_t led_value = 0;

void set_color_output(uint8_t pin, uint8_t color, uint16_t b) {
  b *= color;
  uint8_t c = b >> 8;
  color = color/4 + c/4*3;
  analogWrite(pin, color);
}

void led_setup() {
	pinMode(PIN_R, OUTPUT);
	pinMode(PIN_G, OUTPUT);
	pinMode(PIN_B, OUTPUT);

	//
	//// 50 Hz interrupts via Timer 2
	//
	// Timer1 controls the pins, so we don't touch that.
	//

	cli(); // Disable interrupts

	// Page 45 of the manual.
	// Disable power saving.
	PRR &= ~(_BV(PRTIM2));
	// Page 158 of the manual.
	// Set the clock source to an internal source.
	ASSR &= ~(_BV(AS2));

	// Page 141 of the manual.
	// Clear Control Register A - Don't output a PWM signal.
	TCCR2A = 0; 
	TCCR2B = 0; 

	TCCR2A |= _BV(COM2A1);

	// Page 153, 155, 156
	// _BV(WGM21) : Set PWM mode to CTC
	TCCR2A = _BV(WGM21);

	// From page 147, we calculate cycles by 
	// F_timer = F_CPU/(2 * prescaler * (1 + OCR2A))
	// OCR2A = F_CPU/(2 * prescaler * F_timer) - 1
	// We want this to be 50 Hz.
	// This also sets the top value:
	// OCR2A = F_CPU/(2 * 1024 * 50) - 1;
	OCR2A = 155;

	// Enable the timer interrupt
	//TIMSK2 = _BV(TOIE2);
	TIMSK2 = _BV(OCIE2A);
	// Now whenever the timer overflows, TIMER2_OVF_vect is called.

	// _BV(CS11) : Set the prescaler to /1024, which also enables the timer.
	TCCR2B |= _BV(CS20) | _BV(CS21) | _BV(CS22);

	sei(); // Enable interrupts

	led_set_value(127);
}

void led_set_value (uint8_t value) {
    led_value = value;
    led_phase_inc = (led_value >> 6);
    if (led_alarm) {
      led_alarm = 0;
      led_phase = 0;
    }
}

void led_set_alarm () {
	led_alarm = 1;
    led_phase = 0;
    led_phase_inc = 12;
}

ISR(TIMER2_COMPA_vect) {
	// Advance phase by phase_inc
	led_phase += max(led_phase_inc, 1);

	// Update the LED
	uint8_t *color = (led_alarm)?LED_COLOR_ALARM:LED_COLOR(led_value);
	uint16_t brightness_adjust = LED_BRIGHTNESS(led_phase);
	set_color_output(PIN_R, *(color + 0), brightness_adjust);
	set_color_output(PIN_G, *(color + 1), brightness_adjust);
	set_color_output(PIN_B, *(color + 2), brightness_adjust);
}
