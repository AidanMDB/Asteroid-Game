/*
 * LCD_Driver.c
 *
 *  Created on: May 4, 2025
 *      Author: adann
 */

#include "LCD_Driver.h"

static uint8_t lcd_dma_byte;
static volatile bool lcd_dma_ready = true;
static uint16_t LCD_HEIGHT;
static uint16_t LCD_WIDTH;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
	lcd_dma_ready = true;
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);

	//HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);			// flashes led to show data transfer
	//HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
}



/**
 * @brief returns the height of the LCD in its current orientation
 *
 * */
uint16_t LCD_get_height() {
	return LCD_HEIGHT;
}



/**
 * @brief returns the width of the LCD in its current orientation
 *
 * */
uint16_t LCD_get_width() {
	return LCD_WIDTH;
}





/**
 * @brief Initialize the SPI and backlight for the LCD
 * */
void LCD_SPI_Init() {
	  MX_DMA_Init();
	  MX_SPI2_Init();

	  MX_TIM15_Init();
	  HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
	  htim15.Instance->CCR1 = 10000;
}



/**
 * @brief write a commmand to LCD
 * @param command - command byte being sent
 *
 * */
void LCD_Write_Command(uint8_t command) {
	while(!lcd_dma_ready){};
	lcd_dma_byte = command;
	lcd_dma_ready = false;

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(&hspi2, &lcd_dma_byte, 1);
}



/**
 * @brief write a single byte of data to LCD
 * @param data - byte of data being sent
 *
 * */
void LCD_Write_Data(uint8_t data) {
	while(!lcd_dma_ready){};
	lcd_dma_byte = data;
	lcd_dma_ready = false;

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit_DMA(&hspi2, &lcd_dma_byte, 1);
}


/**
 * @brief write an array of data to LCD
 * @param data_buffer - the array of data to be sent to LCD
 * @param size - the amount of data in bytes being sent
 *
 * */
void LCD_write_pixel_data(uint8_t* data_buffer, uint16_t size) {
	while(!lcd_dma_ready) {}
	//lcd_dma_byte = data;
	lcd_dma_ready = false;

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_SPI_Transmit_DMA(&hspi2, data_buffer, size);
}



/**
 *	@brief sets the brightness level of the LCD
 * 	@param level - brightness of LCD backlight
 *
 */
void LCD_Brightness(uint16_t level) {
	htim15.Instance->CCR1 = level;
}


/**
 * @brief performs a hardware reset on the LCD display
 *
 */
void LCD_Hardware_Reset() {
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);

	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);

	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}



/**
 * @brief determines orientation of the LCD for 0,0 and pixel refresh order
 * @param rotation - determines rotation of the display
 * 		SCREEN_VERTICAL_SD_BOTTOM 	- vertical orientation with SD pins at the bottom
 * 		SCREEN_VERTICAL_SD_TOP		- vertical orientation with SD pins at the top
 * 		SCREEN_HORIZONTAL_SD_RIGHT	- horizontal orientation with SD pins to the right
 * 		SCREEN_HORIZONTAL_SD_LEFT	- horizontal orientation with SD pins to the left
 * */
void LCD_rotation(uint8_t rotation) {

	LCD_Write_Command(SCREEN_ORIENTATION);

	switch (rotation) {
		case SCREEN_VERTICAL_SD_BOTTOM:
			LCD_Write_Data(0x80|0x08);
			LCD_HEIGHT = 320;
			LCD_WIDTH = 240;
			break;
		case SCREEN_VERTICAL_SD_TOP:
			LCD_Write_Data(0x40|0x08);
			LCD_HEIGHT = 320;
			LCD_WIDTH = 240;
			break;
		case SCREEN_HORIZONTAL_SD_RIGHT:
			LCD_Write_Data(0x80|0x08);
			LCD_HEIGHT = 240;
			LCD_WIDTH = 320;
			break;
		case SCREEN_HORIZONTAL_SD_LEFT:
			LCD_Write_Data(0x80|0x08);
			LCD_HEIGHT = 240;
			LCD_WIDTH = 320;
			break;

		default:
			break;
	}
}

/**
 * @brief Initialization of LCD screen
 * */
void LCD_Init() {

	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	LCD_SPI_Init();
	LCD_Hardware_Reset();

	// software reset
	LCD_Write_Command(LCD_SOFTWARE_RST);
	HAL_Delay(10);


	// power control A
	LCD_Write_Command(0xCB);
	LCD_Write_Data(0x39);
	LCD_Write_Data(0x2C);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x34);
	LCD_Write_Data(0x02);

	//POWER CONTROL B
	LCD_Write_Command(0xCF);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0xC1);
	LCD_Write_Data(0x30);

	//DRIVER TIMING CONTROL A
	LCD_Write_Command(0xE8);
	LCD_Write_Data(0x85);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x78);

	//DRIVER TIMING CONTROL B
	LCD_Write_Command(0xEA);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);

	//POWER ON SEQUENCE CONTROL
	LCD_Write_Command(0xED);
	LCD_Write_Data(0x64);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x12);
	LCD_Write_Data(0x81);

	//PUMP RATIO CONTROL
	LCD_Write_Command(0xF7);
	LCD_Write_Data(0x20);

	//POWER CONTROL,VRH[5:0]
	LCD_Write_Command(0xC0);
	LCD_Write_Data(0x23);

	//POWER CONTROL,SAP[2:0];BT[3:0]
	LCD_Write_Command(0xC1);
	LCD_Write_Data(0x10);

	//VCM CONTROL
	LCD_Write_Command(0xC5);
	LCD_Write_Data(0x3E);
	LCD_Write_Data(0x28);

	//VCM CONTROL 2
	LCD_Write_Command(0xC7);
	LCD_Write_Data(0x86);

	//MEMORY ACCESS CONTROL
	LCD_Write_Command(0x36);
	LCD_Write_Data(0x48);

	//PIXEL FORMAT
	LCD_Write_Command(0x3A);
	LCD_Write_Data(0x55);

	//FRAME RATIO CONTROL, STANDARD RGB COLOR
	LCD_Write_Command(0xB1);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x18);

	//DISPLAY FUNCTION CONTROL
	LCD_Write_Command(0xB6);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x82);
	LCD_Write_Data(0x27);

	//3GAMMA FUNCTION DISABLE
	LCD_Write_Command(0xF2);
	LCD_Write_Data(0x00);

	//GAMMA CURVE SELECTED
	LCD_Write_Command(0x26);
	LCD_Write_Data(0x01);

	//POSITIVE GAMMA CORRECTION
	LCD_Write_Command(0xE0);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x31);
	LCD_Write_Data(0x2B);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x4E);
	LCD_Write_Data(0xF1);
	LCD_Write_Data(0x37);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x00);

	//NEGATIVE GAMMA CORRECTION
	LCD_Write_Command(0xE1);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x14);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x11);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x31);
	LCD_Write_Data(0xC1);
	LCD_Write_Data(0x48);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x31);
	LCD_Write_Data(0x36);
	LCD_Write_Data(0x0F);





	// exit sleep mode
	LCD_Write_Command(SLEEP_OUT);
	HAL_Delay(10);

	// turn on display
	LCD_Write_Command(DISPLAY_ON);


	// starting rotation
	LCD_rotation(SCREEN_VERTICAL_SD_TOP);
}





/**
 * @brief block of pixels in LCD memory to write to
 * @param x1 top left x value of block
 * @param y1 top left y value of block
 * @param x2 bottom right x value of block
 * @param y2 bottom right y value of block
 *
 * */
void LCD_set_address_block(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {

	assert(x1 <= LCD_WIDTH && "x1 out of bounds");
	assert(x2 <= LCD_WIDTH && "x2 out of bounds");
	assert(y1 <= LCD_HEIGHT && "y1 out of bounds");
	assert(y2 <= LCD_HEIGHT && "y1 out of bounds");

	LCD_Write_Command(MEMORY_BLOCK_COLUMNS);
	LCD_Write_Data(x1>>8);
	LCD_Write_Data(x1);
	LCD_Write_Data(x2>>8);
	LCD_Write_Data(x2);

	LCD_Write_Command(MEMORY_BLOCK_ROWS);
	LCD_Write_Data(y1>>8);
	LCD_Write_Data(y1);
	LCD_Write_Data(y2>>8);
	LCD_Write_Data(y2);

	LCD_Write_Command(MEMORY_BLOCK_PIXELS);
}





