
#include "graphics.h"


static uint8_t framebuffer[FRAMEBUFFER_SIZE] = {0};



static inline void draw_pixel_buffer(uint16_t x, uint16_t y) {
	LCD_set_address_block(x, y, x, y);
	LCD_write_pixel_data(framebuffer, 2);
}



void draw_pixel(uint16_t x, uint16_t y, uint16_t color) {
	framebuffer[0] = color >> 8;
	framebuffer[1] = color;

	draw_pixel_buffer(x, y);
}




void draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {

	if (w * h * 2 > FRAMEBUFFER_SIZE) {
		return;
	}

	uint8_t color_high = color >> 8;
	uint8_t color_low = color;

	LCD_set_address_block(x, y, x + w - 1, y + h - 1);


	for (int i = 0; i < w * h; i++) {
		framebuffer[ i * 2] = color_high;
		framebuffer[ i * 2 + 1 ] = color_low;
	}

	LCD_write_pixel_data(framebuffer, w * h * 2);
}



void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

	// for perfectly vertical or horizontal lines improve efficiency
	if (x1 == x2 || y1 == y2) {
		return draw_rectangle(x1, y1, x2 - x1, y2 - y1, color);
	}


	framebuffer[0] = color >> 8;
	framebuffer[1] = color;

	int dx = abs(x2 - x1);
	int sx = x1 < x2 ? 1 : -1;

	int dy = -abs(y2 - y1);
	int sy = y1 < y2 ? 1 : -1;

	int err = dx + dy;

	while (true) {
		draw_pixel_buffer(x1, y1);

		int e2 = 2 * err;

		if (e2 >= dy) {
			if (x1 == x2) break;

			err = err + dy;
			x1 = x1 + sx;
		}

		if (e2 <= dx) {
			if (y1 == y2) break;

			err = err + dx;
			y1 = y1 + sy;
		}
	}
}


void draw_circle_outline(uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
	framebuffer[0] = color >> 8;
	framebuffer[1] = color;

	int t1 = r / 16;
	int xVal = r;
	int yVal = 0;

	while(xVal >= yVal) {
		draw_pixel_buffer(x + xVal, y + yVal);
		draw_pixel_buffer(x + xVal, y - yVal);
		draw_pixel_buffer(x - xVal, y - yVal);
		draw_pixel_buffer(x - xVal, y + yVal);
		draw_pixel_buffer(x + yVal, y + xVal);
		draw_pixel_buffer(x + yVal, y - xVal);
		draw_pixel_buffer(x - yVal, y - xVal);
		draw_pixel_buffer(x - yVal, y + xVal);

		yVal++;
		t1 += yVal;
		int t2 = t1 - xVal;
		if (t2 >= 0) {
			t1 = t2;
			xVal--;
		}
	}
}


void draw_custom_block(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* frame) {

	LCD_set_address_block(x, y, x + w - 1, y + h - 1);
	LCD_write_pixel_data(frame, w * h * 2);
}


void fillscreen(uint16_t color) {
	uint16_t screen_height = LCD_get_height();
	uint16_t screen_width = LCD_get_width();

	for (int i = 0; i < screen_height; i = i + 3) {
		draw_rectangle(0, i, screen_width, 3, color);
	}
}





