#include "display.h"

void display_setup(){
	ssd1306_setup();
	
	ssd1306_command_begin();
  	// Set the addressing mode to vertical:
  	ssd1306_command(0x20);
  	ssd1306_command(0b01);
	ssd1306_command_end();

  	display_clear();
}

void display_clear() {
  display_roi_clear();

  ssd1306_data_begin();
  uint16_t i = SSD1306_COLUMN_COUNT * (SSD1306_PAGE_COUNT);
  while(i--)
    ssd1306_data(0x00);
  ssd1306_data_end();
}

void display_roi(uint8_t col, uint8_t page, uint8_t cols, uint8_t pages) {
  ssd1306_command_begin();

  // Set the column range:
  ssd1306_command(0x21);
  ssd1306_command(col);
  cols = cols - 1 + col;
  ssd1306_command(cols);

  // Set the page range:
  ssd1306_command(0x22);
  ssd1306_command(page);
  pages = pages - 1 + page;
  ssd1306_command(pages);

  ssd1306_command_end();	
}

void display_draw(const uint8_t **data, uint8_t n1, uint8_t n2){
  ssd1306_data_begin();
  fastSPIwriteArray((uint8_t *) data, n1 * n2);
  ssd1306_data_end();
}

void display_draw(const uint8_t *data, uint8_t n){
  ssd1306_data_begin();
  fastSPIwriteArray(data, n);
  ssd1306_data_end();
}

void display_draw(uint8_t data){
  ssd1306_data_begin();
  fastSPIwrite(data);
  ssd1306_data_end();
}


