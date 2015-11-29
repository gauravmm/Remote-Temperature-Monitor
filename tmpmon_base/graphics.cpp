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
uint8_t print_text_upscaled_helper(char* text, const uint8_t font [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space, const uint8_t UPSCALE_FACTOR);
int string_width(char text [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space);


void graphics_setup() {
  display_setup();
}

// Celsius to Farenheit
float ctof(float c) {
  return 9.0*c/5.0 + 32.0;
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


void graphics_print_alt(float val, uint8_t units, uint8_t showTempAlarm, float alarmTemp, uint8_t alarmSetIcon) {
  uint8_t GFX_UPSCALE_FACTOR = GFX_UPSCALE_FACTOR_ALARM;
  char tmpbuf[GFX_SCALE_BUF_SZ];

  if (units == GFX_UNIT_F) {
    val = ctof(val);
    if (showTempAlarm)
      alarmTemp = ctof(alarmTemp);
  }

  char unitstr [] = "C";
  if(units==GFX_UNIT_F)
    unitstr[0] = 'F';

  // Display the temperature alarm
  if(showTempAlarm) {
    display_roi(0, DISPLAY_PAGES - FONT_16X11_HEIGHT, DISPLAY_WIDTH, FONT_16X11_HEIGHT);

    uint8_t pos = 0;
    display_set_xor(0xFF);
    
    // Print the alarm string
    graphics_scale_tick(0, GFX_PADDING, GFX_TICK_NUL);
    graphics_float2str(tmpbuf, alarmTemp, units, true);
    pos = GFX_PADDING + print_text_helper(tmpbuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);

    if (alarmSetIcon) {
      graphics_scale_tick(0, DISPLAY_WIDTH - SYMBOL_TEMP_WIDTH - GFX_PADDING - pos, GFX_TICK_NUL);
      display_draw_progmem(SYMBOL_TEMP, SYMBOL_TEMP_WIDTH * SYMBOL_TEMP_HEIGHT);
      graphics_scale_tick(0, GFX_PADDING, GFX_TICK_NUL);  
    } else {
      // Fill the rest with blanks
      graphics_scale_tick(pos, DISPLAY_WIDTH, GFX_TICK_NUL);
    }

    display_set_xor(0x00);
  } else {
    //graphics_scale_tick(0, DISPLAY_WIDTH, GFX_TICK_NUL);
    GFX_UPSCALE_FACTOR = GFX_UPSCALE_FACTOR_NOALARM;
  }

  
  // Display the units
  int unitPos = string_width(unitstr, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
  display_roi(DISPLAY_WIDTH - unitPos, 0, unitPos, FONT_16X11_HEIGHT);
  print_text_helper(unitstr, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);

  // Clear the area below the units
  display_roi(DISPLAY_WIDTH - unitPos, FONT_16X11_HEIGHT, unitPos, (GFX_UPSCALE_FACTOR - 1) * FONT_16X11_HEIGHT);
  graphics_repeat(0x00, unitPos * (GFX_UPSCALE_FACTOR - 1) * FONT_16X11_HEIGHT);

  // Display the temperature.
  graphics_float2str(tmpbuf, val, units, false);
  int tmpwd = string_width(tmpbuf, FONT_16X11_END, FONT_16X11_HEIGHT, 1);

  // Upscale the font
  tmpwd *= GFX_UPSCALE_FACTOR;
  display_roi(0, 0, DISPLAY_WIDTH - unitPos, FONT_16X11_HEIGHT * GFX_UPSCALE_FACTOR);
  // Use a loop to prevent underflow
  for (uint8_t i = tmpwd + GFX_PADDING + unitPos; i < DISPLAY_WIDTH; ++i) {
    graphics_repeat(0x00, GFX_UPSCALE_FACTOR * FONT_16X11_HEIGHT);
  }
  print_text_upscaled_helper(tmpbuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1, GFX_UPSCALE_FACTOR);
  graphics_repeat(0x00, GFX_UPSCALE_FACTOR * FONT_16X11_HEIGHT * GFX_PADDING);

  display_roi_clear();
}

void graphics_print_main(uint8_t *graph, float val, float vmin, float vmax, uint8_t units, uint8_t showTempAlarm, float alarmTemp, uint8_t alarmSetIcon) {
  // Convert units if necessary
  if (units == GFX_UNIT_F) {
    val = ctof(val);
    vmin = ctof(vmin);
    vmax = ctof(vmax);
    if (showTempAlarm)
      alarmTemp = ctof(alarmTemp);
  }

  char scalebuf [GFX_SCALE_BUF_SZ];
  int scalewd = 0;

  // Draw the top:
  display_roi(0, 0, DISPLAY_WIDTH, FONT_16X11_HEIGHT);
  
  display_set_xor(0xFF);
  uint8_t pos = 0;

  if(showTempAlarm) {
    graphics_float2str(scalebuf, alarmTemp, units, true);
    pos = print_text_helper(scalebuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
  }

  graphics_float2str(scalebuf, val, units, true);
  scalewd = string_width(scalebuf, FONT_16X11_END, FONT_16X11_HEIGHT, 1);

  graphics_scale_tick(0, (DISPLAY_WIDTH - scalewd) / 2 - pos, GFX_TICK_NUL);
  print_text_helper(scalebuf, FONT_16X11, FONT_16X11_END, FONT_16X11_HEIGHT, 1);
  pos = (DISPLAY_WIDTH - scalewd) / 2 + scalewd;

  if(alarmSetIcon) {
    graphics_scale_tick(0, DISPLAY_WIDTH - SYMBOL_TEMP_WIDTH - GFX_PADDING - pos, GFX_TICK_NUL);
    display_draw_progmem(SYMBOL_TEMP, SYMBOL_TEMP_WIDTH * SYMBOL_TEMP_HEIGHT);
    graphics_scale_tick(0, GFX_PADDING, GFX_TICK_NUL);
  } else {
    graphics_scale_tick(0, DISPLAY_WIDTH - pos, GFX_TICK_NUL);
  }
  
  display_set_xor(0x00);

  // Different visual styles
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


uint8_t print_text_upscaled_helper(char* text, const uint8_t font [], uint16_t font_end [], uint8_t font_height, uint8_t gutter_space, const uint8_t UPSCALE_FACTOR) {
  const uint8_t UPSCALE_HEIGHT = font_height * UPSCALE_FACTOR;
  uint8_t i = 0;
  uint8_t printed_cols = 0;
  char currchar;
  
  // Fail if we don't meet the height requirement.
  if(font_height != 2)
    return 0;

  uint8_t colbuf_a[UPSCALE_HEIGHT];
  uint8_t *colbuf = colbuf_a;

#if GFX_UPSCALE_SMOOTH == 1
  uint8_t colbuf_b[UPSCALE_HEIGHT];
  uint8_t *colbuf_prev = colbuf_b;
#endif  
  
  uint8_t inpbuf[font_height];

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
      for(char j = 0; j < gutter_space * UPSCALE_FACTOR; ++j){
          graphics_repeat(0, UPSCALE_HEIGHT);
          printed_cols += 1;
      }
    }

    // Get the offset of this item:
    uint16_t offset = get_char_offset(font_end, currchar);
    // Get the number of bytes in the glyph.
    uint8_t len = get_char_bytes(font_end, currchar);

#if GFX_UPSCALE_SMOOTH == 1
    for (uint8_t j = 0; j < UPSCALE_HEIGHT; ++j) {
      colbuf_prev[j] = 0;
    }
#endif

    for(uint8_t i = 0; i < len; i += font_height) {
      memcpy_P(inpbuf, font + offset + i, font_height);
      // Upscale font from inpbuf to colbuf:
      for (uint8_t j = 0; j < UPSCALE_HEIGHT; ++j) {
        colbuf[j] = 0;
      }

      // This code will work for any integer upscaling factor.
      for (uint8_t j = 0; j < FONT_16X11_HEIGHT * 8; ++j) {
        uint8_t bit = inpbuf[j / 8] & _BV(j % 8);
        if(bit)
          for (uint8_t k = j * UPSCALE_FACTOR; k < (j + 1) * UPSCALE_FACTOR; ++k)
            colbuf[k / 8] |=  _BV(k % 8);
      }

#if GFX_UPSCALE_SMOOTH == 1
      // Store the OR of the previous character and the current
      // Output that as the first col.
      for (uint8_t j = 0; j < UPSCALE_HEIGHT; ++j)
        colbuf_prev[j] |= colbuf[j];

      display_draw(colbuf_prev, UPSCALE_HEIGHT);
      for (uint8_t j = 1; j < UPSCALE_FACTOR; ++j)
        display_draw(colbuf, UPSCALE_HEIGHT);

      // Swap current and previous buffer
      uint8_t *tmp = colbuf_prev;
      colbuf_prev = colbuf;
      colbuf = tmp;
#else 
      for (uint8_t j = 0; j < UPSCALE_FACTOR; ++j)
        display_draw(colbuf, UPSCALE_HEIGHT);
#endif
    }
    
    printed_cols += (len/font_height) * UPSCALE_FACTOR;
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
