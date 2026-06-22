// by Pytel

#ifndef __GUI_h__
#define __GUI_h__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"
#include "game.h"

bool render_game (game_t * game);
bool render_menu (menu_t *menu, game_t * game);
void render_score_bord (menu_t *menu, game_t * game);
void render_logo (uint16_t * img);

/* Vypise na displey v textovem rezimu menu
 *
 */
void print_menu(menu_t *menu, game_t * game);

void score_bord(menu_t *menu, game_t * game);

// RGB to 16b color
bool RBG_to_16b (color_t RGB, uint16_t *color);
bool open_img (char * file_name, unsigned char ** img);
bool save_logo (unsigned char *img, game_t *game);

#endif
/* end of gui.h */