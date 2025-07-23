/*
 * LCD_Driver.h
 *
 *  Created on: May 4, 2025
 *      Author: adann
 */

#ifndef INC_LCD_DRIVER_H_
#define INC_LCD_DRIVER_H_


#include "spi.h"
#include "dma.h"
#include "tim.h"
#include "stdbool.h"
#include "assert.h"


#define LCD_SOFTWARE_RST 		0x01
#define SLEEP_OUT 				0x11
#define DISPLAY_ON      		0x29
#define SCREEN_ORIENTATION		0x36
#define MEMORY_BLOCK_COLUMNS 	0x2A
#define MEMORY_BLOCK_ROWS		0x2B
#define MEMORY_BLOCK_PIXELS		0x2C

#define SCREEN_VERTICAL_SD_TOP		0
#define SCREEN_VERTICAL_SD_BOTTOM	1
#define SCREEN_HORIZONTAL_SD_RIGHT	2
#define SCREEN_HORIZONTAL_SD_LEFT	3

uint16_t LCD_get_height();
uint16_t LCD_get_width();

void LCD_SPI_Init();
void LCD_Write_Command(uint8_t command);
void LCD_Write_Data(uint8_t data);
void LCD_write_pixel_data(uint8_t* data_buffer, uint16_t size);
void LCD_Brightness(uint16_t level);
void LCD_Hardware_Reset();
void LCD_Rotation(uint8_t rotation);
void LCD_Init();
void LCD_set_address_block(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
void LCD_write_line();



#endif /* INC_LCD_DRIVER_H_ */
