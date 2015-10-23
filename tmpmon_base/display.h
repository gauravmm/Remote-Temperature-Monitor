#ifndef DISPLAY_H
#define DISPLAY_H

#include "ssd1306.h"

#define DISPLAY_WIDTH SSD1306_LCDWIDTH

void display_setup();
void display_clear();

// Set the region of interest of the screen. All subsequent drawing instructions are sent there.
void display_roi(uint8_t col, uint8_t page, uint8_t cols, uint8_t pages);

// Clear the ROI 
#define display_roi_clear() display_roi(0, 0, SSD1306_COLUMN_COUNT, SSD1306_PAGE_COUNT)

// Set a XOR filter
#define display_set_xor(pf) ssd1306_set_xor(pf)

// Send n1*n2 bytes instructions to the display
void display_draw(const uint8_t **data, uint8_t n1, uint8_t n2);

// Send n bytes instructions to the display
void display_draw(const uint8_t *data, uint8_t n);

// Send 1 byte instructions to the display
void display_draw(uint8_t data);

#endif
