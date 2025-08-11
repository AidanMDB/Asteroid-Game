


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


#define MAX_NUM_LASERS 5
#define LASER_COLOR 0x07E0
typedef struct _Laser{
	uint16_t x;
	uint16_t y;
} Laser;


typedef struct _Asteroid {
	uint16_t x;
	uint16_t y;
} Asteroid;


void game_init();
void move(int8_t direction);
void shoot_laser();
void increase_score();
void update_score_board();
void draw_asteroid();

#endif
