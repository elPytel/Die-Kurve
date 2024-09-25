// by Pytel

#ifndef __GAME_h__
#define __GAME_h__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h> 

typedef struct {
	uint8_t R;
	uint8_t G;
	uint8_t B;
} color_t;

typedef struct {
	int y;
	int x;
} position_t;

typedef struct {
	bool enable;	// ucastni se hry
	bool alive;		// still not dead
	// barva
	int color_index;
	uint16_t color;
	// pozice
	position_t position;
	// vektor		- posun v osach X a Y
	position_t vector;
	position_t old_vector;
} player_t;

typedef struct {
	bool playing;
	// hraci
	int players;		// pocet hracu
	player_t player1;
	player_t player2; 
	player_t player3;
	// boti
	int bots;			// počet bot ve hře
	position_t *positions;
	position_t *vectors;
	uint16_t *colors;	// pole barev botu
	bool *live_bots;
	int *directions;
	int *score;			// [players+bots] pole, ktere drzi skore pro kazdeho hrace i bota
	int *spaces;	// cerchovany mod
	int speed;		// turn time
	uint16_t *game_bord;	// veskera herni plocha - aka frame buffer
	uint16_t *logo;			// zde je nahrane herni logo
	uint16_t *frame_buffer;	// frame buffer pro vice vrstva meny
} game_t;


// vrati true pokud porad probiha hra
bool playing (game_t * game);

// inicialiuje strukturu game_t
bool game_init (game_t * game);

// nastavi hodnotu playning na true pokud existuje zivy hrac/bot
void someon_alive (game_t * game);

// aktualizuje vektro bota
void AI_move (game_t * game);

// aktualizuje vektor hrace
void player_move (game_t * game);

/* skontroluje validitu tahu
 * vyhodnoti mrtve hrace a udeli jim body, posune zive ve smeru vektoru
 */
void valid_play(game_t * game);

int make_score(game_t * game);

// dekonstruktor
void free_game (game_t * game);

void kill_game_all (game_t * game);

// po vyjeti z menu na polozku start se zahaji hra s nastavenymi parametry.

#endif
/* end of game.h */