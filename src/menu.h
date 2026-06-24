// by Pytel

#ifndef __MENU_h__
#define __MENU_h__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "game.h"

/**
 * @brief menu structure
 * 
 * Structure representing the languages for the game.
 */
typedef enum {
	EN,
	CZ,
	GE,
	PL,
	NBR_LANGUAGE
} languages_t;

typedef enum {
	TRUE_STR,
	FALSE_STR,
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
	const char **item_names;
	const char **color_names;
	color_t *colors;
	int step;
	int selected_player;
	int x_shift;
	int speed;
	// jazyky
	int language;
	const char **languages;
	const char **words;
	int size;
} menu_t;


/**
 * @brief Prepares a new game based on menu data.
 * @param menu Pointer to the menu structure.
 * @param game Pointer to the game structure.
 * @return true if the game is prepared successfully, false otherwise.
 */
bool set_game (menu_t * menu, game_t * game);

/**
 * @brief Inicializuje výchozí hodnoty menu.
 * @param menu Ukazatel na strukturu menu.
 * @return true při úspěchu, false při chybě.
 */
bool menu_init (menu_t * menu);

/**
 * @brief Spustí a odbaví logiku menu (vykreslení, reakce na enkodéry).
 */
void menu_function (menu_t * menu, game_t * game);

/**
 * @brief Destruktor menu, uvolní alokované resources (barvy).
 */
void kill_menu (menu_t * menu);


#endif
/* end of menu.h */