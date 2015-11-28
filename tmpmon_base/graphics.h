// Graphics support, including drawing fonts.
// Some typesetting support from previous work:
// https://github.com/gauravmm/HT1632-for-Arduino/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>

void graphics_setup();

void graphics_print_main(uint8_t *graph, float val, float vmin, float vmax, uint8_t units);
void graphics_print_menu(char** menu, uint8_t highlight);

#define GFX_LEFT  0
#define GFX_RIGHT 1
#define GFX_UNIT_C 0
#define GFX_UNIT_F 1

#endif GRAPHICS_H