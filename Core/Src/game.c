


#include "game.h"

static uint16_t spaceship_curr_loc = SPACESHIP_START_X;
static volatile uint16_t score = 0;
static uint8_t* number[] = {number_0, number_1, number_2, number_3, number_4, number_5, number_6, number_7, number_8, number_9};
//static uint16_t score = 0;



void game_init() {
	fillscreen(BACKGROUND_COLOR);
	draw_custom_block(SPACESHIP_START_X, SPACESHIP_ROW, SPACESHIP_WIDTH, SPACESHIP_HEIGHT, spaceship_sprite);
	update_score_board();

	//while(1) {

	//}

}







void move(int8_t direction) {
	draw_custom_block(spaceship_curr_loc + direction, SPACESHIP_ROW, SPACESHIP_WIDTH, SPACESHIP_HEIGHT, spaceship_sprite);

	if (direction < 0) {
		draw_rectangle(spaceship_curr_loc + SPACESHIP_WIDTH, SPACESHIP_ROW, -1 * direction, SPACESHIP_HEIGHT, BACKGROUND_COLOR);
	}
	else {
		draw_rectangle(spaceship_curr_loc, SPACESHIP_ROW, direction, SPACESHIP_HEIGHT, BACKGROUND_COLOR);
	}

	spaceship_curr_loc += direction;
}



void move_laser() {

}



void shoot_laser() {
	//draw_rectangle(space_curr_loc + (SPACESHIP_WIDTH / 2), );
}


void increase_score() {
	score++;
}


void update_score_board() {
	uint16_t temp = score;

	for (uint8_t i = 0; i < 3; i++) {
		uint8_t num = temp % 10;
		temp /= 10;
		draw_custom_block((SCORE_OFFSET * (3 - i)) + (NUMBER_WIDTH * (2 - i)), SCORE_OFFSET, NUMBER_WIDTH, NUMBER_HEIGHT, number[num]);
	}
}
