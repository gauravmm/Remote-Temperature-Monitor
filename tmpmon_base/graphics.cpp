// Graphics
// Draw things to parts of the screen.

#include "font_16x11.h"
#include "display.h"


uint8_t get_char_offset(uint16_t font_end [], uint8_t font_index);
uint8_t get_char_bytes(uint16_t font_end [], uint8_t font_index);
void print_menu_text(char text [], uint8_t page, uint8_t highlight);
uint8_t print_text_helper(char text [], const uint8_t font [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space);


void graphics_setup() {
	display_setup();
}

void graphics_print_menu(char** menu, uint8_t highlight) {
	// display_clear();
	
	for (uint8_t i = 0; i < 4; ++i) {
		char* item = *(menu + i);
		if (item) {
			print_menu_text(item, i * 2, highlight==i);
		} else {
			break;
		}
	}
}

void print_menu_text(char* text, uint8_t page, uint8_t highlight) {
	display_set_xor(highlight?0xFF:0x00);
	display_roi(0, page, DISPLAY_WIDTH, FONT_16X11_HEIGHT);
	uint8_t printed_cols = print_text_helper(text, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 2);

	// Fill the remainder of the row.
	while(printed_cols++ < DISPLAY_WIDTH) {
		display_draw(FONT_16X11, FONT_16X11_HEIGHT);
	}
	display_set_xor(0x00);
}

uint8_t print_text_helper(char* text, const uint8_t font [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space) {
	uint8_t i = 0;
	uint8_t printed_cols = 0;
	char currchar;
	
	while(true){  
		if(text[i] == '\0')
			break;
                
		currchar = text[i] - 32;
		if(currchar >= 65 && currchar <= 90) // If character is lower-case, automatically make it upper-case
			currchar -= 32; // Make this character uppercase.

		if(currchar < 0 || currchar >= 64) { // If out of bounds, skip
			++i;
			continue; // Skip this character.
		}

                if(i > 0) {
                  // Draw the gutter space
        		for(char j = 0; j < gutter_space; ++j){
        			display_draw(font, font_height);
        			printed_cols += 1;
        		}
                }


		// Get the offset of this item:
		uint16_t offset = get_char_offset(font_end, currchar);
		// Get the number of bytes in the glyph.
		uint8_t len = get_char_bytes(font_end, currchar);

		display_draw(font + offset, len);
		printed_cols += len/2;
		
		++i;
	}

	return printed_cols;
}

// Gives you the width, in columns, of a particular string.
int string_width(const char text [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space) {
	uint8_t wd = 0;
	char i = 0;
	char currchar;
	
	while(true){  
		if (text[i] == '\0') {
			return wd - gutter_space;
		}
			
		currchar = text[i] - 32;
		if (currchar >= 65 && currchar <= 90) { // If character is lower-case, automatically make it upper-case
			currchar -= 32;                     // Make this character uppercase.
		}

		if (currchar < 0 || currchar >= 64) {   // If out of bounds, skip
			++i;
			continue; // Skip this character.
		}

		wd += get_char_bytes(font_end, currchar)/font_height + gutter_space;
		++i;
	}
}

uint8_t get_char_offset(int font_end [], uint8_t font_index) {
	if(font_index == 0) {
		return 0;
	}
	// The width is the difference between the ending index of
	//  this and the previous characters:
	return font_end[font_index - 1];
}

uint8_t get_char_bytes(int font_end [], uint8_t font_index) {
	if(font_index == 0) {
		return font_end[0];
	}
	// The width is the difference between the ending index of
	//  this and the previous characters:
	return (font_end[font_index] - font_end[font_index - 1]);
}
