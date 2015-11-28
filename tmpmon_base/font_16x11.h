/* 
 * 116-high FONT FOR RENDERING TO SSD113006 SCREEN.
 * 8 pixels-per-byte, variable width.
 *
 * By Gaurav Manek, 200114.
 * Based on https://github.com/gauravmm/HT11632-for-Arduino/tree/master/Arduino/HT11632
 */

#ifndef __FONT16X11_H
#define __FONT16X11_H

#include <avr/pgmspace.h>

#define FONT_16X11_HEIGHT  2

extern const uint8_t FONT_16X11 [] PROGMEM;

// Index of first byte after the glyph
extern uint16_t FONT_16X11_END [];

extern const uint8_t SYMBOL_TEMP [] PROGMEM;
extern uint16_t SYMBOL_TEMP_WIDTH;
extern uint16_t SYMBOL_TEMP_HEIGHT;

#endif
