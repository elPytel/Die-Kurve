// by Pytel

#ifndef __MENU_h__
#define __MENU_h__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef enum {
	EN,
	CZ,
	GE,
	PL,
	NBR_LANGUAGE
} languages_t;

typedef enum {
	TRUE,
	FALSE,
	PLAYERI,
	BOT,
	SCORE,
	CONTINUE,
	NBR_WORD
} name_space_t;

typedef enum {
	LANGUAGE,
	TEXT_SIZE,
	NUMBER_OF_PLAYERS,
	PLAYER,
	COLOR,
	NUMBER_OF_BOTS,
	DIFFICULTY,
	RESTART,
	START,
	NBR_ITEM
} sub_menu_t;

typedef enum {
	RED,
	GREEN,
	BLUE,
	YELLOW,
	CYAN,
	MAGENTA,
	GREY,
	WHITE,
	NBR_COLOR
} color_name_t;

#include "game.h"

typedef struct {
	bool start;
	bool restart;
	// Y set
	uint8_t last_positon_w0;
	uint8_t new_positon_w0;
	// X set
	uint8_t last_positon_w1;
	uint8_t new_positon_w1;
	// polozka
	sub_menu_t item;
	char **item_names;
	char **color_names;
	color_t *colors;
	int step;
	int selected_player;
	int x_shift;
	int speed;
	// jazyky
	int language;
	char **languages;
	char **words;
	int size;
} menu_t;


// podle dat z menu pripravi novou hru
bool set_game (menu_t * menu, game_t * game);

// inicializuje menu
bool menu_init (menu_t * menu);

void menu_function (menu_t * menu, game_t * game);

// dekonstruktor
void kill_menu (menu_t * menu);


#endif
/* end of menu.h */