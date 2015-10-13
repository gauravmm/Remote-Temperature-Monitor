// Basic drawing commands for the SSD1306 screen

#include "ssd1306.h"

// Write directly to ports, using 
volatile uint8_t *mosiport, *clkport, *csport, *dcport;
uint8_t mosipinmask, clkpinmask, cspinmask, dcpinmask;


// This must be called before the display can be used.
// Initialization sequence, adapted from:
// http://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples
void ssd1306_setup(){
	uint8_t cs 		= PIN_SSD1306_CS;
	uint8_t rst 	= PIN_SSD1306_RESET;
	uint8_t dc 		= PIN_SSD1306_DC;
	uint8_t sclk 	= PIN_SSD1306_CLK;
	uint8_t sid 	= PIN_SSD1306_MOSI;

	// Set pin modes
	pinMode(sid, 	OUTPUT);
	pinMode(sclk,	OUTPUT);
	pinMode(dc, 	OUTPUT);
	pinMode(cs, 	OUTPUT);
	clkport     = portOutputRegister(digitalPinToPort(sclk));
	clkpinmask  = digitalPinToBitMask(sclk);
	mosiport    = portOutputRegister(digitalPinToPort(sid));
	mosipinmask = digitalPinToBitMask(sid);
	csport      = portOutputRegister(digitalPinToPort(cs));
	cspinmask   = digitalPinToBitMask(cs);
	dcport      = portOutputRegister(digitalPinToPort(dc));
	dcpinmask   = digitalPinToBitMask(dc);

	// Begin the setup by resetting the display:
	pinMode(rst, OUTPUT);
	digitalWrite(rst, HIGH);
	delay(1);
	digitalWrite(rst, LOW);
	delay(10);
	digitalWrite(rst, HIGH);
	
	ssd1306_command_begin();

	// Init sequence for 128x64 OLED module
	ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
	ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
	ssd1306_command(0x80);                                  // the suggested ratio 0x80
	ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
	ssd1306_command(0x3F);
	ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
	ssd1306_command(0x0);                                   // no offset
	ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
	ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
	ssd1306_command(0x14); // Use internal charge-pump
	ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
	ssd1306_command(0x01);                                  // Top-Down, Left-Right
	ssd1306_command(SSD1306_SEGREMAP | 0x1);
	ssd1306_command(SSD1306_COMSCANDEC);
	ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
	ssd1306_command(0x12);
	ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
	ssd1306_command(0xCF); // Set contrast

	ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
	ssd1306_command(0xF1); // Pre-charge period

	ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
	ssd1306_command(0x40);
	ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
	ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
	ssd1306_command(SSD1306_DISPLAYON);                     //--turn on oled panel

	ssd1306_command_end();
}
