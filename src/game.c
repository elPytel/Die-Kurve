// by Pytel

#include "game.h"
#include "driver.h"
#include "gui.h"
#include "logger.h"

#define DEBUG 1

bool game_playing (game_t * game) { return game->playing; }

void player_init (player_t *player) {
	player->enable = false;
	player->alive = false;
	player->encoder_id = -1;
	player->color_index = 0;
	player->color = 0;
	// pozice
	player->position.y = 0;
	player->position.x = 0;
	// vektor
	player->vector.x = 0;
	player->vector.y = 0;
	// old vektor
	player->old_vector.x = 0;
	player->old_vector.y = 0;
}

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

bool game_init (game_t * game) {
	game->playing = false;
	// hraci
	game->active_players_count = 0;		// pocet hracu
	for (int i = 0; i < MAX_PLAYERS; i++) {
		player_init(&game->players[i]);
		game->players[i].encoder_id = i;
		game->players[i].color_index = i;
	}
	// boti
	game->bots = 0;			// počet bot ve hře
	game->positions = NULL;
	game->directions = NULL;
	game->vectors = NULL;
	game->colors = NULL;	// pole barev botu
	game->live_bots = NULL;
	game->score = NULL;	
	game->spaces = NULL;
	// game
	game->game_bord = NULL;
	game->logo = NULL;
	game->frame_buffer = NULL;
	// random seed
	srand(time(0));
	return true;
}

void player_to_string(player_t *p, char *buffer, size_t buf_size) {
    snprintf(buffer, buf_size, 
             "Player [Enable: %d, Alive: %d, Pos: (%d, %d), Vec: (%d, %d), ColorIdx: %d]", 
             p->enable, p->alive, 
             p->position.x, p->position.y, 
             p->vector.x, p->vector.y, 
             p->color_index);
}

void right (int *dy, int *dx) {
	int x = *dx;
	int y = *dy;
	if ( y == 1 && x == 0 ) {
		*dy = 1;
		*dx = -1;
	} else if ( y == 1 && x == 1 ) {
		*dy = 1;
		*dx = 0;
	} else if ( y == 0 && x == 1 ) {
		*dy = 1;
		*dx = 1;
	} else if ( y == -1 && x == 1 ) {
		*dy = 0;
		*dx = 1;
	} else if ( y == -1 && x == 0 ) {
		*dy = -1;
		*dx = 1;
	} else if ( y == -1 && x == -1 ) {
		*dy = -1;
		*dx = 0;
	} else if ( y == 0 && x == -1 ) {
		*dy = -1;
		*dx = -1;
	} else if ( y == 1 && x == -1 ) {
		*dy = 0;
		*dx = -1;
	}
}

void left (int *dy, int *dx) {
	int x = *dx;
	int y = *dy;
	if ( y == 1 && x == 0 ) {
		*dy = 1;
		*dx = 1;
	} else if ( y == 1 && x == 1 ) {
		*dy = 0;
		*dx = 1;
	} else if ( y == 0 && x == 1 ) {
		*dy = -1;
		*dx = 1;
	} else if ( y == -1 && x == 1 ) {
		*dy = -1;
		*dx = 0;
	} else if ( y == -1 && x == 0 ) {
		*dy = -1;
		*dx = -1;
	} else if ( y == -1 && x == -1 ) {
		*dy = 0;
		*dx = -1;
	} else if ( y == 0 && x == -1 ) {
		*dy = 1;
		*dx = -1;
	} else if ( y == 1 && x == -1 ) {
		*dy = 1;
		*dx = 0;
	}
}

#define F 450
#define LEFT 5
#define RIGHT 5
#define LL 5
#define RR 5
#define EVADE 80
void move (game_t *game, int *dy, int *dx, int i, int rand) {
	int step = rand % (F+LEFT+RIGHT+LL+RR);
	*dy = game->vectors[i].y;
	*dx = game->vectors[i].x;
	if (step < F) {						
		// continue with out change
		game->directions[i] = 0;
		if (DEBUG) {
			printf("Forward!");
		}
	} else if ( step >= F && step < F+LEFT) {	// left
		left (dy, dx);
		game->directions[i] = -1;
		if (DEBUG) {
			printf("Left!");
		}
	} else if ( step >= F+LEFT && step < F+LEFT+RIGHT) {	// right
		right (dy, dx);
		game->directions[i] = 1;
		if (DEBUG) {
			printf("Right!");
		}
	} else if ( step >= F+LEFT+RIGHT && step < F+LEFT+RIGHT+LL) {	// left left
		left (dy, dx);
		left (dy, dx);
		game->directions[i] = -2;
		if (DEBUG) {
			printf("Left, left!");
		}
	} else if ( step >= F+LEFT+RIGHT+LL && step < F+LEFT+RIGHT+LL+RR) {	// right right
		right (dy, dx);
		right (dy, dx);
		game->directions[i] = 2;
		if (DEBUG) {
			printf("Right, right!");
		}
	} else {
		if (DEBUG) {
			printf("ERROR!");
		}
	}
}

bool next_step (game_t *game, int *x1, int *y1, int *x2, int *y2, int *x3, int *y3, int dx, int dy, int i) {
	if (game->directions[i] > 2 || game->directions[i] < -2) {
		return false;
	}
	if (game->directions[i] == 2) {	// right, right!
		// left
		left (&dy, &dx);
		left (&dy, &dx);
		*x3 = game->positions[i].x + dx;
		*y3 = game->positions[i].y + dy;
		
		// midle
		right (&dy, &dx);
		*x1 = game->positions[i].x + dx;
		*y1 = game->positions[i].y + dy;
		
		// right
		right (&dy, &dx);
		*y2 = game->positions[i].y + dy;
		*x2 = game->positions[i].x + dx;
	} else if (game->directions[i] == -2) {	// left, left!
		// left
		*x3 = game->positions[i].x + dx;
		*y3 = game->positions[i].y + dy;
		
		// midle
		right (&dy, &dx);
		*x1 = game->positions[i].x + dx;
		*y1 = game->positions[i].y + dy;
		
		// right
		right (&dy, &dx);
		*y2 = game->positions[i].y + dy;
		*x2 = game->positions[i].x + dx;
		left (&dy, &dx);
	} else {
		// left
		left (&dy, &dx);
		*y3 = game->positions[i].y + dy;
		*x3 = game->positions[i].x + dx;
		
		// midle
		right (&dy, &dx);
		*x1 = game->positions[i].x + dx;
		*y1 = game->positions[i].y + dy;
		
		// right
		right (&dy, &dx);
		*y2 = game->positions[i].y + dy;
		*x2 = game->positions[i].x + dx;
		left (&dy, &dx);
	}
	return true;
}

void game_AI_move (game_t * game) {
	// kazdy bot
	for (int i = 0; i < game->bots; i++) {
		if (game->live_bots[i]) {	// dany bot je na zivu
			if (DEBUG) {
				printf("Bot number: %d play: ", i);
			}
			bool change = true;
			int random = 0;
			game->directions[i] = 0;
			bool bonus = false;
			// pozice
			// midle
			int x1;
			int y1;
			// right
			int x2;
			int y2;
			// left
			int x3;
			int y3;
			// vektor
			int dx;
			int dy;
			while (change) {		// snaha o zmenu vektoru
				change = false;
				if ( bonus ) {		// nehraje rovne
					random = rand()%(LEFT+RIGHT+LL+RR) + F;
				} else {
					random = rand()%(F+LEFT+RIGHT+LL+RR);
				}
				uint16_t pixel1 = 0;
				uint16_t pixel2 = 0;
				uint16_t pixel3 = 0;
				
				uint16_t pixel4 = 0;
				uint16_t pixel5 = 0;
				uint16_t pixel6 = 0;
				// new vektor
				move(game, &dy, &dx, i, random);
				
				// next step
				next_step(game, &x1, &y1, &x2, &y2, &x3, &y3, dx, dy, i);
				
				// validity
				if ( x1>WIDTH-3 || y1>HEIGHT-3 || x1<3 || y1<3 || x2>WIDTH-2 || y2>HEIGHT-2 || x2<2 || y2<2 || x3>WIDTH-2 || y3>HEIGHT-2 || x3<2 || y3<2 ) {
					change = (rand() % 100) < EVADE ? true : false;
					bonus = true;
				} else {
					// jeden pred
					pixel1 = game->game_bord[y1*WIDTH +x1];
					pixel2 = game->game_bord[y2*WIDTH +x2];
					pixel3 = game->game_bord[y3*WIDTH +x3];
					// dava pred
					pixel4 = game->game_bord[(y1+2*dy)*WIDTH +(x1+2*dx)];
					pixel5 = game->game_bord[(y2+2*dy)*WIDTH +(x2+2*dx)];
					pixel6 = game->game_bord[(y3+2*dy)*WIDTH +(x3+2*dx)];
				}
				
				if ( pixel1 != 0 || (pixel2 != 0 && pixel2 != game->colors[i]) || (pixel3 != 0 && pixel3 != game->colors[i]) || pixel4 != 0 || pixel5 != 0 || pixel6 != 0 ) {	// obsazene pole
					// change
					if (DEBUG) {
						printf(" Not free space! ");
						//printf(" Color: %d", pixel);
					}
					change = (rand() % 100) < EVADE ? true : false;
					bonus = true;
				}
			}
			// zahraje tah
			game->vectors[i].y = dy;
			game->vectors[i].x = dx;
			if (DEBUG) {
				printf("\n");
			}
		}
	}
}

void degree_to_vector (int degree, int *y, int *x) {
	if ( degree < 16 || degree >= 239 ) {
		*y = 1;
		*x = 0;
	} else if ( degree < 48 && degree >= 16 ) {
		*y = 1;
		*x = -1;
	} else if ( degree < 80 && degree >= 48 ) {
		*y = 0;
		*x = -1;
	} else if ( degree < 112 && degree >= 80 ) {
		*y = -1;
		*x = -1;
	} else if ( degree < 144 && degree >= 112 ) {
		*y = -1;
		*x = 0;
	} else if ( degree < 176 && degree >= 144 ) {
		*y = -1;
		*x = 1;
	} else if ( degree < 208 && degree >= 176 ) {
		*y = 0;
		*x = 1;
	} else if ( degree < 239 && degree >= 208 ) {
		*y = 1;
		*x = 1;
	}
}

void player_move(game_t *game) {
    uint8_t degree;

    // Iterujeme přes všechny možné sloty hráčů
    for (int i = 0; i < MAX_PLAYERS; i++) {
        player_t *p = &game->players[i]; // Ukazatel na aktuálního hráče

        // Přeskočíme, pokud hráč neexistuje nebo je mrtvý
        if (!p->enable || !p->alive) {
            continue;
        }

        // 1. Získání úhlu z příslušného enkodéru
        // Tady využíváme tvé stávající API encoder_position
        if (encoder_position(p->encoder_id, &degree)) {
            
            // 2. Uložení starého vektoru
            p->old_vector.y = p->vector.y;
            p->old_vector.x = p->vector.x;

            // 3. Výpočet nového vektoru
            degree_to_vector((int)degree, &p->vector.y, &p->vector.x);

            // 4. Inicializace vektoru (pokud je první tah)
            if (p->old_vector.y == 0 && p->old_vector.x == 0) {
                p->old_vector.y = p->vector.y;
                p->old_vector.x = p->vector.x;
            }

            if (DEBUG) {
                printf("Player %d: dy: %d dx: %d\n", i, p->vector.y, p->vector.x);
				logger_log("Player %d: dy: %d dx: %d", i, p->vector.y, p->vector.x);
            }
        }
    }
}

int direction_change (int dy, int dx, int dy_old, int dx_old) {
	//printf("NEW dy: %d	dx: %d\n", dy, dx);
	int i = -2;
	int oldy = dy_old;
	int oldx = dx_old;
	//printf("Old dy: %d	dx: %d\n", oldy, oldx);
	left (&oldy, &oldx);
	//printf("Old dy: %d	dx: %d\n", oldy, oldx);
	left (&oldy, &oldx);
	//printf("Old dy: %d	dx: %d\n", oldy, oldx);
	//printf("\n");
	while ( !(dy == oldy && dx == oldx) ) {
		i++;
		//printf("olddy: %d =? dy: %d	olddx: %d =? dx: %d\n", oldy, dy, oldx, dx);
		right (&oldy, &oldx);
		if ( i > 2) {
			return i;
		}
	}
	//printf("olddy: %d = dy: %d	olddx: %d = dx: %d\n", oldy, dy, oldx, dx);
	return i;
}

bool player_invalid_move (int direction, uint16_t pixel1, uint16_t pixel2, uint16_t pixel3, uint16_t color) {
	bool ret;
	printf("Player moved: ");
	switch (direction) {
		case -2:	// left, left!
			ret = ( (pixel1 != 0 && pixel1 != color) || pixel2 != 0 || (pixel3 != 0 && pixel3 != color) );
			if (DEBUG) {
				printf("Left, left!\n");
			}
			break;
		case -1:	// left
			ret = ( pixel1 != 0 || pixel2 != 0 || (pixel3 != 0 && pixel3 != color) );
			if (DEBUG) {
				printf("Left\n");
			}
			break;
		case 0:	// forvard
			ret = (pixel1 != 0 || pixel2 != 0 || pixel3 != 0);
			if (DEBUG) {
				printf("Forward\n");
			}
			break;
		case 1:	// right
			ret = (pixel1 != 0 || (pixel2 != 0 && pixel2 != color) || pixel3 != 0);
			if (DEBUG) {
				printf("Right\n");
			}
			break;
		case 2:	// right, right!
			ret = ( (pixel1 != 0 && pixel1 != color) || (pixel2 != 0 && pixel2 != color) || pixel3 != 0);
			if (DEBUG) {
				printf("Right, right!\n");
			}
			break;
		default:
			ret = true;
			break;
	}
	return ret;
}

void game_validate_play (game_t * game) {
	for (int i = 0; i < MAX_PLAYERS; i++) {
        player_t *p = &game->players[i];
        
        if (!p->enable || !p->alive) continue;

        int dy = p->vector.y;
        int dx = p->vector.x;

        // Výpočet pozic (střed, levá, pravá pro detekci kolizí)
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;
		int y = p->position.y;
		int x = p->position.x;
		// nova pozice
		// left
		left (&dy, &dx);
		y3 = y + dy;
		x3 = x + dx;
		// midle
		right (&dy, &dx);
		y1 = y + dy;
		x1 = x + dx;
		// right
		right (&dy, &dx);
		y2 = y + dy;
		x2 = x + dx;
		left (&dy, &dx);	//reset

        // Validace hranic
        if (x1 >= WIDTH || y1 >= HEIGHT || x1 < 0 || y1 < 0 || 
            x2 >= WIDTH || y2 >= HEIGHT || x2 < 0 || y2 < 0 || 
            x3 >= WIDTH || y3 >= HEIGHT || x3 < 0 || y3 < 0) {
            
            p->alive = false;
            game->score[i] += game_calculate_score(game);
        } else {
            // Kontrola kolizí s již existující čarou
            uint16_t p1 = game->game_bord[y1 * WIDTH + x1];
            uint16_t p2 = game->game_bord[y2 * WIDTH + x2];
            uint16_t p3 = game->game_bord[y3 * WIDTH + x3];

            if (player_invalid_move(direction_change(dy, dx, p->old_vector.y, p->old_vector.x), p1, p2, p3, p->color)) {
                p->alive = false;
                game->score[i] += game_calculate_score(game);
            } else {
                // Tah je validní, kreslíme
                game->game_bord[y1 * WIDTH + x1] = p->color;
                game->game_bord[y2 * WIDTH + x2] = p->color;
                game->game_bord[y3 * WIDTH + x3] = p->color;
                p->position.y = y1;
                p->position.x = x1;
            }
        }
    }
	
	// valid and play bot
	// kazdy bot
	uint16_t pixel1 = 0;
	uint16_t pixel2 = 0;
	uint16_t pixel3 = 0;
	// pozice
	// midle
	int x1;
	int y1;
	// right
	int x2;
	int y2;
	// left
	int x3;
	int y3;
	// vektor
	int dy;
	int dx;
	for (int i = 0; i < game->bots; i++) {
		if (game->live_bots[i]) {	// dany bot je na zivu
			// vektor
			dy = game->vectors[i].y;
			dx = game->vectors[i].x;
			// pozice
			next_step (game, &x1, &y1, &x2, &y2, &x3, &y3, dx, dy, i);
			
			// validity
			if ( x1>=WIDTH || y1>=HEIGHT || x1<0 || y1<0 || x2>=WIDTH || y2>=HEIGHT || x2<0 || y2<0 || x3>=WIDTH || y3>=HEIGHT || x3<0 || y3<0 ) {
				game->live_bots[i] = false;
				game->score[i+game->active_players_count] = game->score[i+game->active_players_count] + game_calculate_score(game);
			} else {
				pixel1 = game->game_bord[y1*WIDTH +x1];
				pixel2 = game->game_bord[y2*WIDTH +x2];
				pixel3 = game->game_bord[y3*WIDTH +x3];
			}
			if ( pixel1 != 0 || (pixel2 != 0 && pixel2 != game->colors[i]) || (pixel3 != 0 && pixel3 != game->colors[i]) ) {	// obsazene pole
				game->live_bots[i] = false;
				game->score[i+game->active_players_count] = game->score[i+game->active_players_count] + game_calculate_score(game);	
			} else {
				// zahral tah
				game->game_bord[y1*WIDTH +x1] = game->colors[i];
				game->game_bord[y2*WIDTH +x2] = game->colors[i];
				game->game_bord[y3*WIDTH +x3] = game->colors[i];
				game->positions[i].y = y1;
				game->positions[i].x = x1;
			}
		}
	}
	
	// zije jeste nekdo?
	game->playing = game_is_someon_alive(game);
	
	if (DEBUG && !game_playing(game) ) {
		printf("Everybody's dead dave!\n");
	}
}

bool game_is_someon_alive (game_t * game) {
	// hraci
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (game->players[i].enable && game->players[i].alive) {
			return true;
		}
	}
	
	// boti
	for (int i = 0; i < game->bots; i++) {
		if (game->live_bots[i]) {	// dany bot je na zivu
			return true;
		}
	}
	return false;
}

void game_set_if_someon_alive (game_t * game) {
	game->playing = game_is_someon_alive(game);
}

int game_calculate_score(game_t * game) {
	// spocita kolik je mrtvych hracu a botu a tu hodnotu vrati
	int score = 0;
	
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (game->players[i].enable && game->players[i].alive == false) {
			score++;
		}
	}
	
	// boti
	for (int i = 0; i < game->bots; i++) {
		if (!game->live_bots[i]) {	// dany bot je mrtvy
			score++;
		}
	}
	return score;
}

void game_free (game_t * game) {
	if (game->positions) {
		free(game->positions);
		game->positions = NULL;
	}
	if (game->vectors) {
		free(game->vectors);
		game->vectors = NULL;
	}
	if (game->colors) {
		free(game->colors);
		game->colors = NULL;
	}
	if (game->directions) {
		free(game->directions);
		game->directions = NULL;
	}
	if (game->live_bots) {
		free(game->live_bots);
		game->live_bots = NULL;
	}
	if (game->score) {
		free(game->score);
		game->score = NULL;
	}
	if (game->game_bord) {
		free(game->game_bord);
		game->game_bord = NULL;
	}
	if (game->spaces) {
		free(game->spaces);
		game->spaces= NULL;
	}
}

void game_kill_all (game_t * game) {
	game_free (game);
	if (game->frame_buffer) {
		free(game->frame_buffer);
		game->frame_buffer = NULL;
	}
	if (game->logo) {
		free(game->logo);
		game->logo = NULL;
	}
}

/*
void degree_to_vector (int degree, int *y, int *x) {
	if ( degree < 16 || degree >= 239 ) {
		*y = -1;
		*x = 0;
	} else if ( degree < 48 && degree >= 16 ) {
		*y = -1;
		*x = -1;
	} else if ( degree < 80 && degree >= 48 ) {
		*y = 0;
		*x = -1;
	} else if ( degree < 112 && degree >= 80 ) {
		*y = 1;
		*x = -1;
	} else if ( degree < 144 && degree >= 112 ) {
		*y = 1;
		*x = 0;
	} else if ( degree < 176 && degree >= 144 ) {
		*y = 1;
		*x = 1;
	} else if ( degree < 208 && degree >= 176 ) {
		*y = 0;
		*x = 1;
	} else if ( degree < 239 && degree >= 208 ) {
		*y = -1;
		*x = 1;
	}
}*/
/* end of game.c */
