


#include "game.h"

static uint16_t spaceship_curr_loc = SPACESHIP_START_X;
static volatile uint16_t score = 0;
static uint8_t* number[] = {number_0, number_1, number_2, number_3, number_4, number_5, number_6, number_7, number_8, number_9};
static Laser lasers[MAX_NUM_LASERS];
static Asteroid asteroid = {0, 0};
static uint32_t rng_state = 1;
//static uint16_t score = 0;


void rng_LCG() {
	rng_state = (rng_state * 1664525 + 1013904223) % 200;
}




void move_asteroid(uint8_t speed) {
	rng_LCG();
	asteroid.x = rng_state;
	draw_custom_block(asteroid.x, asteroid.y, ASTEROID_SIZE, ASTEROID_SIZE, asteroid_sprite);
	asteroid.y -= speed;
}



void init_laser() {
	for (int i = 0; i < MAX_NUM_LASERS; i++) {
		lasers[i].x = 0;
		lasers[i].y = 0;
	}
}


void game_init() {
	fillscreen(BACKGROUND_COLOR);
	draw_custom_block(SPACESHIP_START_X, SPACESHIP_ROW, SPACESHIP_WIDTH, SPACESHIP_HEIGHT, spaceship_sprite);
	update_score_board();
	init_laser();
	//while(1) {

	//}

}







void move(int8_t direction) {

	// check its not going off the screen
	if (direction > 0 && spaceship_curr_loc + direction + SPACESHIP_WIDTH >= LCD_get_width()) {
		return;
	}
	if (direction < 0 && spaceship_curr_loc + direction < 0) {
		return;
	}

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
	for (int i = 0; i < MAX_NUM_LASERS; i++) {
		if (lasers[i].y != 0) {
			if (lasers[i].y - 2 <= 0) {
				lasers[i].y = 0;
				draw_rectangle(lasers[i].x, lasers[i].y, LASER_WIDTH, LASER_LENGTH, BACKGROUND_COLOR);
				continue;
			}
			lasers[i].y -= 2;
			draw_custom_block(lasers[i].x, lasers[i].y, LASER_WIDTH, LASER_LENGTH, laser_sprite);
			draw_rectangle(lasers[i].x, lasers[i].y + LASER_LENGTH + 2, LASER_WIDTH, 2, BACKGROUND_COLOR);
		}
	}
}



void shoot_laser() {
	for (int i = 0; i < MAX_NUM_LASERS; i++) {
		if (lasers[i].y == 0) {
			lasers[i].y = SPACESHIP_ROW - LASER_LENGTH;
			lasers[i].x = spaceship_curr_loc + ((SPACESHIP_WIDTH - 1) / 2);
			return;
		}
	}
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
