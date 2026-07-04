
#ifndef __BOT_H__
#define __BOT_H__

#include "game.h"

#define MAX_BOTS 10

typedef struct {
	bool alive;
	position_t position;
	position_t vector;
	uint16_t color;
	int direction;
} bot_t;

void bot_init (bot_t *bot);
void bot_set_random_position (bot_t *bot, int width, int height);
void bot_set_random_vector (bot_t *bot);
void bot_set_random_color (bot_t *bot);
bool bots_random_setup (bot_t *bots, int num_bots, int width, int height);

#endif