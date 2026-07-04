
#include "bot.h"
#include "gui.h"

void bot_init (bot_t *bot) {
	bot->alive = false;
	bot->color = 0;
	// pozice
	bot->position.y = 0;
	bot->position.x = 0;
	// vektor
	bot->vector.x = 0;
	bot->vector.y = 0;
	bot->direction = 0;
}

void bot_set_random_position (bot_t *bot, int width, int height) {
    bot->position.x = 5 + (rand() % (width-10) );
    bot->position.y = 5 + (rand() % (height-10) );
}

void bot_set_random_vector (bot_t *bot) {
    bot->vector.x = 0;
    bot->vector.y = 0;
    while (bot->vector.y == 0 && bot->vector.x == 0 ) {
        bot->vector.x = rand()%3 -1;
        bot->vector.y = rand()%3 -1;
    }
}

void bot_set_random_color (bot_t *bot) {
    color_t color;
    color.R = (rand() % 200) + 50;	// 255, 200 + 55, 155 + 100
    color.G = (rand() % 200) + 50;
    color.B = (rand() % 200) + 50;
    RBG_to_16b(color, &bot->color);
}

bool bots_random_setup (bot_t *bots, int num_bots, int width, int height) {
    if (bots == NULL) {
        fprintf(stderr, "ERROR: bots pointer is NULL!\n");
        return false;
    }
    if (num_bots <= 0 || num_bots > MAX_BOTS) {
        fprintf(stderr, "ERROR: Invalid number of bots: %d. Must be between 1 and %d.\n", num_bots, MAX_BOTS);
        return false;
    }

    for (int i = 0; i < num_bots; i++) {
        bot_init(&bots[i]);
        bot_set_random_position(&bots[i], width, height);
        bot_set_random_vector(&bots[i]);
        bot_set_random_color(&bots[i]);
    }
    
    return true;
}
