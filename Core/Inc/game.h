


#ifndef GAME_H
#define GAME_H

#include <sprites.h>
#include "gpio.h"
#include "graphics.h"
#include "LCD_Driver.h"

#define BACKGROUND_COLOR 0x0000


#define SPACESHIP_ROW 320 - 38
#define SPACESHIP_START_X 101


#define SCORE_OFFSET 2


#define LASER_COLOR 0x07E0
#define LASER_LENGTH 10
typedef struct _Laser{

	uint16_t x_location;
	uint16_t y_location;
} Laser;



typedef struct _star{

} Star;



void game_init();
void move(int8_t direction);
void shoot_laser();
void increase_score();
void update_score_board();

#endif
