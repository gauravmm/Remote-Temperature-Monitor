// Commands taken from:
// http://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples
// To save on memory, I am not using a library, instead directly drawing pixels.
//

#ifndef SSD1306_H
#define SSD1306_H

#include <Arduino.h>
#include "config_base.h"

//
// Screen Parameters
//

#define SSD1306_LCDWIDTH                 128
#define SSD1306_LCDHEIGHT                 64

#define SSD1306_COLUMN_COUNT			 128
#define SSD1306_PAGE_COUNT				(SSD1306_LCDHEIGHT/8)


#define SSD1306_SETCONTRAST 			0x81
#define SSD1306_DISPLAYALLON_RESUME 	0xA4
#define SSD1306_DISPLAYALLON			0xA5
#define SSD1306_NORMALDISPLAY			0xA6
#define SSD1306_INVERTDISPLAY			0xA7
#define SSD1306_DISPLAYOFF				0xAE
#define SSD1306_DISPLAYON				0xAF

#define SSD1306_SETDISPLAYOFFSET		0xD3
#define SSD1306_SETCOMPINS				0xDA

#define SSD1306_SETVCOMDETECT 			0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 		0xD5
#define SSD1306_SETPRECHARGE 			0xD9

#define SSD1306_SETMULTIPLEX 			0xA8

#define SSD1306_SETLOWCOLUMN 			0x00
#define SSD1306_SETHIGHCOLUMN 			0x10

#define SSD1306_SETSTARTLINE 			0x40

#define SSD1306_MEMORYMODE 				0x20

#define SSD1306_COMSCANINC 				0xC0
#define SSD1306_COMSCANDEC 				0xC8

#define SSD1306_SEGREMAP 				0xA0

#define SSD1306_CHARGEPUMP 				0x8D

#define SSD1306_EXTERNALVCC 			0x01
#define SSD1306_SWITCHCAPVCC 			0x02

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 		0x2F
#define SSD1306_DEACTIVATE_SCROLL 		0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL  0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL   0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A


//
// Function Prototypes
//
void ssd1306_setup();

// NO-OP Definition
#define NOP(); __asm__("nop\n\t"); 
// This provides ~62.5ns delay per NOP. 

/* All these extern declarations and inline declarations are designed
 * to squeeze out timing performance in exchange for a (sligntly) larger
 * program size.
 */

extern volatile uint8_t *mosiport, *clkport, *csport, *dcport;
extern uint8_t mosipinmask, clkpinmask, cspinmask, dcpinmask;
extern uint8_t xorfilter;

inline void fastSPIwrite(uint8_t d) {
  for(uint8_t bit = 0x80; bit; bit >>= 1) {
    if(d & bit) *mosiport |=  mosipinmask; // MOSI to HIGH
    else        *mosiport &= ~mosipinmask; // MOSI to LOW
    *clkport |=  clkpinmask; 	// Clock to HIGH
    NOP();
    *clkport &= ~clkpinmask; 	// Clock to LOW
  }
}

inline void ssd1306_data_array(const uint8_t* arr, uint8_t len) {
  while(len--)
    fastSPIwrite((*(arr++)) ^ xorfilter);
}


// Send a command to the screen.
// Pulls the DC line to low before writing a command.
inline void ssd1306_command(uint8_t c) {
  fastSPIwrite(c);
}

// Must be called before a sequence of calls to ssd1306_command.
inline void ssd1306_command_begin() {
	*csport |= cspinmask;  //digitalWrite(cs, HIGH);
  *dcport &= ~dcpinmask; //digitalWrite(dc, LOW);
  *csport &= ~cspinmask; //digitalWrite(cs, LOW);
}

inline void ssd1306_command_end() {
	*csport |= cspinmask;  //digitalWrite(cs, HIGH);
  	*dcport &= ~dcpinmask; //digitalWrite(dc, LOW);
}

// Must be called before sending display data to the screen.
inline void ssd1306_data_begin() {
	*csport |= cspinmask;  //digitalWrite(cs, HIGH);
  	*dcport |= dcpinmask;  //digitalWrite(dc, HIGH);
  	*csport &= ~cspinmask; //digitalWrite(cs, LOW);
}

inline void ssd1306_data(uint8_t c) {
  fastSPIwrite(c ^ xorfilter);
}

inline void ssd1306_data_end() {
  *csport |= cspinmask; //digitalWrite(cs, HIGH);
}

inline void ssd1306_set_xor(uint8_t filter) {
  xorfilter = filter;
}

#endif

