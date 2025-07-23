



#ifndef GRPAHICS_H
#define GRAPHICS_H

#include "stdint.h"
#include "stdlib.h"
#include "LCD_Driver.h"

#define FRAMEBUFFER_SIZE 320 * 2 * 3	// 1500 pixels * 2 bytes of 16-bit color


/*
 * 16-bit color order
 * 		Byte 1				Byte 2
 * 	R R R R R G G G		G G G B B B B B
 *
 * **/


void draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void draw_rectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void draw_circle_outline(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
void draw_custom_block(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* frame);
void fillscreen(uint16_t color);

#endif
