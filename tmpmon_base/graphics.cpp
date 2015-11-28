// Graphics
// Draw things to parts of the screen.

#include "graphics.h"
#include "font_16x11.h"
#include "display.h"
#include "GraphData.h"

uint16_t get_char_offset(uint16_t font_end [], uint8_t font_index);
uint8_t get_char_bytes(uint16_t font_end [], uint8_t font_index);
void print_menu_text(char text [], uint8_t page, uint8_t highlight);
uint8_t print_text_helper(char text [], const uint8_t font [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space);
int string_width(char text [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space);


void graphics_setup() {
  display_setup();
}

#define GFX_SCALE_BUF_SZ 6

void graphics_float2str(char *scalebuf, float v, uint8_t units, uint8_t useunits) {
  uint8_t i = 0;
  uint8_t isneg = 0;
  float b = 100.0;
  if (v < 0) {
    scalebuf[i++] = '-';
    v = -v;
    isneg = true;
  } 

  uint8_t printDigit = 0;
  while(b >= 1.0) {
    if (v >= b || printDigit || b == 1.0) {
      printDigit = 1; // Print all subsequent digits
      uint8_t j = floor(v / b);
      if (j > 9) {
        // Something went wrong, the temperature is above 999. Go to max
        goto maxtemp;
      }

      scalebuf[i++] = '0' + j;
      v -= j * b;
    }
    b /= 10.0;
  }

  if(useunits)
    scalebuf[i++] = (units==GFX_UNIT_C)?'C':'F';
  scalebuf[i++] = '\0';

  return;

  maxtemp:
  if (isneg) {
    scalebuf[0] = 'M';
    scalebuf[1] = 'I';
    scalebuf[2] = 'N';
  } else {
    scalebuf[0] = 'M';
    scalebuf[1] = 'A';
    scalebuf[2] = 'X';
  }

  if (useunits) {
    scalebuf[3] = ' ';
    scalebuf[4] = (units==GFX_UNIT_C)?'C':'F';
    scalebuf[5] = '\0';
  } else {
    scalebuf[3] = '\0';
  }
  
}

#define GFX_TICK_NUL 0
#define GFX_TICK_TOP 1
#define GFX_TICK_BOT 2
void graphics_scale_tick(uint8_t currwd, uint8_t desiredwd, uint8_t typ) {
  uint8_t b [2] = {0x00, 0x00};
  if(typ == GFX_TICK_TOP) {
    b[0] = 0b00000011;
  } else if (typ == GFX_TICK_BOT) {
    b[1] = 0b11000000;
  }

  while (currwd < desiredwd) {
    display_draw(b, 2);
    currwd++;
  }
}

void graphics_repeat(uint8_t b, uint8_t c) {
  while (c--)
    display_draw(b);
}

#define GFX_PADDING 6
#define GFX_PADDING_VERT 1
#define GFX_VERT_WD 2

void graphics_print_main(uint8_t *graph, float val, float vmin, float vmax, uint8_t units) {
  char scalebuf [GFX_SCALE_BUF_SZ];
  int scalewd = 0;


  // Draw the top:
  display_roi(0, 0, DISPLAY_WIDTH, FONT_16X11_HEIGHT);
  
  display_set_xor(0xFF);
  uint8_t pos = 0;

  graphics_float2str(scalebuf, val, units, true);
  scalewd = string_width(scalebuf, FONT_16X11_END, FONT_16X11_HEIGHT, 1);

  pos = (DISPLAY_WIDTH - scalewd) / 2;
  graphics_scale_tick(0, pos, GFX_TICK_NUL);
  pos += scalewd;
  print_text_helper(scalebuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);

  graphics_scale_tick(0, DISPLAY_WIDTH - SYMBOL_TEMP_WIDTH - GFX_PADDING - pos, GFX_TICK_NUL);
  display_draw_progmem(SYMBOL_TEMP, SYMBOL_TEMP_WIDTH * SYMBOL_TEMP_HEIGHT);
  graphics_scale_tick(0, GFX_PADDING, GFX_TICK_NUL);
  
  display_set_xor(0x00);

  #if GFX_PADDING_VERT == 0
    // Draw the scale - max:
    graphics_float2str(scalebuf, vmax, units, false);
    scalewd = string_width(scalebuf, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
    display_roi(DATA_LEN, DISPLAY_PAGES - (DATA_HEIGHT/8) + GFX_PADDING_VERT, DISPLAY_WIDTH - DATA_LEN, FONT_16X11_HEIGHT);
    graphics_scale_tick(scalewd, DISPLAY_WIDTH - DATA_LEN, GFX_TICK_TOP);
    print_text_helper(scalebuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
    
    // Draw the scale - min:
    graphics_float2str(scalebuf, vmin, units, false);
    scalewd = string_width(scalebuf, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
    display_roi(DATA_LEN, DISPLAY_PAGES - FONT_16X11_HEIGHT - GFX_PADDING_VERT, DISPLAY_WIDTH - DATA_LEN, FONT_16X11_HEIGHT);
    graphics_scale_tick(scalewd, DISPLAY_WIDTH - DATA_LEN, GFX_TICK_BOT);
    print_text_helper(scalebuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
  #else
    display_roi(DATA_LEN, DISPLAY_PAGES - (DATA_HEIGHT/8), DISPLAY_WIDTH - DATA_LEN, 1);
    graphics_repeat(0b00000110, DISPLAY_WIDTH - DATA_LEN - GFX_VERT_WD - GFX_PADDING);
    graphics_repeat(0xFF - 1, GFX_VERT_WD);
    graphics_repeat(0x00, GFX_PADDING);

    // Draw the scale - max:
    graphics_float2str(scalebuf, vmax, units, false);
    scalewd = string_width(scalebuf, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
    display_roi(DATA_LEN, DISPLAY_PAGES - (DATA_HEIGHT/8) + GFX_PADDING_VERT, DISPLAY_WIDTH - DATA_LEN, FONT_16X11_HEIGHT);
    graphics_scale_tick(scalewd, DISPLAY_WIDTH - DATA_LEN, GFX_TICK_NUL);
    print_text_helper(scalebuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);

    // Draw the scale - min:
    display_roi(DATA_LEN, DISPLAY_PAGES - 1, DISPLAY_WIDTH - DATA_LEN, 1);
    graphics_repeat(0b11000000, DISPLAY_WIDTH - DATA_LEN - GFX_VERT_WD - GFX_PADDING);
    graphics_repeat(0xFF, GFX_VERT_WD);
    graphics_repeat(0x00, GFX_PADDING);
    
    graphics_float2str(scalebuf, vmin, units, false);
    scalewd = string_width(scalebuf, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
    display_roi(DATA_LEN, DISPLAY_PAGES - FONT_16X11_HEIGHT - GFX_PADDING_VERT, DISPLAY_WIDTH - DATA_LEN, FONT_16X11_HEIGHT);
    graphics_scale_tick(scalewd, DISPLAY_WIDTH - DATA_LEN, GFX_TICK_NUL);
    print_text_helper(scalebuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
  #endif


  // Draw the graph
  display_roi(0, DISPLAY_PAGES - (DATA_HEIGHT/8), DATA_LEN, (DATA_HEIGHT/8));
  for(uint8_t i = 0; i < DATA_LEN; ++i) {
    uint8_t val = DATA_HEIGHT - *(graph++);
    for(uint8_t j = 0; j < DATA_HEIGHT/8; ++j) {
      if(!val) {
        display_draw((uint8_t) 0xFF);
      } else if (val >= 8) {
        display_draw((uint8_t) 0x00);
        val -= 8;
      } else {
        display_draw((uint8_t) ~((1 << val) - 1));
        val = 0;
      }
    }
  }
}

void graphics_print_menu(char **menu, uint8_t highlight) {
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
    display_draw_progmem(FONT_16X11, FONT_16X11_HEIGHT);
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
          display_draw_progmem(font, font_height);
          printed_cols += 1;
      }
    }


    // Get the offset of this item:
    uint16_t offset = get_char_offset(font_end, currchar);
    // Get the number of bytes in the glyph.
    uint8_t len = get_char_bytes(font_end, currchar);

    display_draw_progmem(font + offset, len);
    printed_cols += len/2;
    
    ++i;
  }

  return printed_cols;
}

// Gives you the width, in columns, of a particular string.
int string_width(char text [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space) {
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

uint16_t get_char_offset(uint16_t font_end [], uint8_t font_index) {
  if(font_index == 0) {
    return 0;
  }
  // The width is the difference between the ending index of
  //  this and the previous characters:
  return font_end[font_index - 1];
}

uint8_t get_char_bytes(uint16_t font_end [], uint8_t font_index) {
  if(font_index == 0) {
    return font_end[0];
  }
  // The width is the difference between the ending index of
  //  this and the previous characters:
  return (font_end[font_index] - font_end[font_index - 1]);
}
