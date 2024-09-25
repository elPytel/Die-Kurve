// by Pytel

#include "game.h"
#include "driver.h"
#include "gui.h"

#define DEBUG 1

bool playing (game_t * game) { return game->playing; }

void player_init (player_t *player) {
	player->enable = false;
	player->alive = false;
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
	// random seed
	srand(time(0));
}

bool game_init (game_t * game) {
	game->playing = false;
	// hraci
	game->players = 0;		// pocet hracu
	player_init (&game->player1);
	player_init (&game->player2);
	player_init (&game->player3);
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
	return true;
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

void AI_move (game_t * game) {
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
				next_step (game, &x1, &y1, &x2, &y2, &x3, &y3, dx, dy, i);
				
				// validity
				if ( x1>WIDTH-3 || y1>HEIGHT-3 || x1<3 || y1<3 || x2>WIDTH-2 || y2>HEIGHT-2 || x2<2 || y2<2 || x3>WIDTH-2 || y3>HEIGHT-2 || x3<2 || y3<2 ) {
					change = (rand() % 100) < EVADE ? true : false;
					bonus = true;
				} else {
					// jeden pred
					pixel1 = game->game_bord[y1*WIDTH +x1];
					pixel2 = game->game_bord[y2*WIDTH +x2];
					pixel2 = game->game_bord[y3*WIDTH +x3];
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

// PC demo
int old_degree = 0;

void player_move (game_t * game) {
	uint8_t degree = -1;
	// pohyb vsech aktivnich a zivich hracu
	
	if ( game->players > 0 && game->player1.alive == true) {
		// nastaveni noveho vektoru
		// TODO
		// PC demo
		degree = old_degree;
		wheel_position(0, &degree);
		old_degree = degree;
		game->player1.old_vector.y = game->player1.vector.y;
		game->player1.old_vector.x = game->player1.vector.x;
		degree_to_vector ((int)degree, &game->player1.vector.y, &game->player1.vector.x);
		printf("dy: %d	dx: %d - set\n", game->player1.vector.y, game->player1.vector.x);
		// inivializace
		if (game->player1.old_vector.y == 0 && game->player1.old_vector.x == 0) {
			game->player1.old_vector.y = game->player1.vector.y;
			game->player1.old_vector.x = game->player1.vector.x;
			printf("OLD dy: %d	dx: %d	- set\n", game->player1.old_vector.y, game->player1.old_vector.x);
		} else if (DEBUG) {
			printf("OLD dy: %d	dx: %d	- seted\n", game->player1.old_vector.y, game->player1.old_vector.x);
		}
	}
	if ( game->players > 2 && game->player2.alive == true) {
		// nastaveni noveho vektoru
		wheel_position(1, &degree);
		degree_to_vector ((int)degree, &game->player2.vector.y, &game->player2.vector.x);
	}
	if ( game->players > 3 && game->player3.alive == true) {
		// nastaveni noveho vektoru
		wheel_position(2, &degree);
		degree_to_vector ((int)degree, &game->player3.vector.y, &game->player3.vector.x);
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

void valid_play (game_t * game) {
	// promene
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
	
	// zmena smeru hrace
	int direction = 0;
	
	// valid and play player
	if ( game->players > 0 && game->player1.alive == true) {
		// vektor
		dy = game->player1.vector.y;
		dx = game->player1.vector.x;
		// stara pozice 
		int y = game->player1.position.y;
		int x = game->player1.position.x;
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
		// validity
		if ( x1>=WIDTH || y1>=HEIGHT || x1<0 || y1<0 || x2>=WIDTH || y2>=HEIGHT || x2<0 || y2<0 || x3>=WIDTH || y3>=HEIGHT || x3<0 || y3<0 ) {
			// umrel dostane skore
			game->player1.alive = false;
			game->score[0] += make_score(game);
		} else {
			pixel1 = game->game_bord[y1*WIDTH +x1];
			pixel2 = game->game_bord[y2*WIDTH +x2];
			pixel3 = game->game_bord[y3*WIDTH +x3];
			int dy_old = game->player1.old_vector.y;
			int dx_old = game->player1.old_vector.x;
			direction = direction_change(dy, dx, dy_old, dx_old);
		}
		if ( player_invalid_move (direction, pixel1, pixel2, pixel3, game->player1.color) ) {		// obsazene pole
			game->player1.alive = false;
			game->score[0] += make_score(game);	
		} else {
			// zahral tah
			game->game_bord[y1*WIDTH +x1] = game->player1.color;
			game->game_bord[y2*WIDTH +x2] = game->player1.color;
			game->game_bord[y3*WIDTH +x3] = game->player1.color;
			game->player1.position.y = y1;
			game->player1.position.x = x1;
		}
	}
	if ( game->players > 2 && game->player2.alive == true) {
		// vektor
		dy = game->player2.vector.y;
		dx = game->player2.vector.x;
		// stara pozice 
		int y = game->player2.position.y;
		int x = game->player2.position.x;
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
		// validity
		if ( x1>=WIDTH || y1>=HEIGHT || x1<0 || y1<0 || x2>=WIDTH || y2>=HEIGHT || x2<0 || y2<0 || x3>=WIDTH || y3>=HEIGHT || x3<0 || y3<0 ) {
			// umrel dostane skore
			game->player2.alive = false;
			game->score[1] += make_score(game);
		} else {
			pixel1 = game->game_bord[y1*WIDTH +x1];
			pixel2 = game->game_bord[y2*WIDTH +x2];
			pixel3 = game->game_bord[y3*WIDTH +x3];
			int dy_old = game->player2.old_vector.y;
			int dx_old = game->player2.old_vector.x;
			direction = direction_change(dy, dx, dy_old, dx_old);
		}
		if ( player_invalid_move (direction, pixel1, pixel2, pixel3, game->player2.color) ) {		// obsazene pole
			game->player2.alive = false;
			game->score[1] += make_score(game);	
		} else {
			// zahral tah
			game->game_bord[y1*WIDTH +x1] = game->player2.color;
			game->game_bord[y2*WIDTH +x2] = game->player2.color;
			game->game_bord[y3*WIDTH +x3] = game->player2.color;
			game->player2.position.y = y1;
			game->player2.position.x = x1;
		}
	}
	if ( game->players > 3 && game->player3.alive == true) {
		// vektor
		dy = game->player3.vector.y;
		dx = game->player3.vector.x;
		// stara pozice 
		int y = game->player3.position.y;
		int x = game->player3.position.x;
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
		// validity
		if ( x1>=WIDTH || y1>=HEIGHT || x1<0 || y1<0 || x2>=WIDTH || y2>=HEIGHT || x2<0 || y2<0 || x3>=WIDTH || y3>=HEIGHT || x3<0 || y3<0 ) {
			// umrel dostane skore
			game->player3.alive = false;
			game->score[2] += make_score(game);
		} else {
			pixel1 = game->game_bord[y1*WIDTH +x1];
			pixel2 = game->game_bord[y2*WIDTH +x2];
			pixel3 = game->game_bord[y3*WIDTH +x3];
			int dy_old = game->player3.old_vector.y;
			int dx_old = game->player3.old_vector.x;
			direction = direction_change(dy, dx, dy_old, dx_old);
		}
		if ( player_invalid_move (direction, pixel1, pixel2, pixel3, game->player3.color) ) {		// obsazene pole
			game->player3.alive = false;
			game->score[2] += make_score(game);	
		} else {
			// zahral tah
			game->game_bord[y1*WIDTH +x1] = game->player3.color;
			game->game_bord[y2*WIDTH +x2] = game->player3.color;
			game->game_bord[y3*WIDTH +x3] = game->player3.color;
			game->player3.position.y = y1;
			game->player3.position.x = x1;
		}
	}
	
	// valid and play bot
	// kazdy bot
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
				game->score[i+game->players] = game->score[i+game->players] + make_score(game);
			} else {
				pixel1 = game->game_bord[y1*WIDTH +x1];
				pixel2 = game->game_bord[y2*WIDTH +x2];
				pixel3 = game->game_bord[y3*WIDTH +x3];
			}
			if ( pixel1 != 0 || (pixel2 != 0 && pixel2 != game->colors[i]) || (pixel3 != 0 && pixel3 != game->colors[i]) ) {	// obsazene pole
				game->live_bots[i] = false;
				game->score[i+game->players] = game->score[i+game->players] + make_score(game);	
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
	someon_alive (game);
	
	if (DEBUG && !playing (game) ) {
		printf("Everybody's dead dave!\n");
	}
}

void someon_alive (game_t * game) {
	game->playing = false;
	
	// hraci
	if ( game->players > 0 && game->player1.alive == true) {
		game->playing = true;
	}
	if ( game->players > 2 && game->player2.alive == true) {
		game->playing = true;
	}
	if ( game->players > 3 && game->player3.alive == true) {
		game->playing = true;
	}
	
	// boti
	for (int i = 0; i < game->bots; i++) {
		if (game->live_bots[i]) {	// dany bot je na zivu
			game->playing = true;
		}
	}
}

int make_score(game_t * game) {
	// spocita kolik je mrtvych hracu a botu a tu hodnotu vrati
	int score = 0;
	// hraci
	if ( game->players > 0 && game->player1.alive == false) {
		score++;
	}
	if ( game->players > 2 && game->player2.alive == false) {
		score++;
	}
	if ( game->players > 3 && game->player3.alive == false) {
		score++;
	}
	
	// boti
	for (int i = 0; i < game->bots; i++) {
		if (!game->live_bots[i]) {	// dany bot je mrtvy
			score++;
		}
	}
	return score;
}

void free_game (game_t * game) {
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

void kill_game_all (game_t * game) {
	free_game (game);
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
/*

	if (0 && DEBUG) {
		printf(" From: %d:%d To: %d:%d", game->positions[i].y, game->positions[i].x, y1, x1);
	}
	
	 printf("R: %X\n", mix);
	 printf("R+G: %X\n", mix);
	 printf("R+G+B: %X\n", mix);

*/
/* end of game.c */
