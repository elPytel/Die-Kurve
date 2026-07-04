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

/**
 * @brief Renders the menu on the display in text mode.
 * @param menu Pointer to the menu structure.
 * @param game Pointer to the game structure.
 * @return true if rendering was successful, false otherwise.
 */
bool render_menu (menu_t *menu, game_t * game);

/**
 * @brief Renders the logo on the display in graphical mode.
 * @param img Pointer to the image data representing the logo.
 */
void logo_render (uint16_t * img);

/**
 * @brief Translates a string to the appropriate character encoding for display.
 * @param string The input string to be translated.
 * @return A pointer to the translated string, or NULL if the input is empty.
 */
unsigned char* translate(const char *string);

/**
 * @brief Prints the menu on the display in text mode.
 * @param menu Pointer to the menu structure.
 * @param game Pointer to the game structure.
 */
void print_menu(menu_t *menu, game_t * game);

/**
 * @brief Renders the score board on the display in graphical mode.
 * @param menu Pointer to the menu structure.
 * @param game Pointer to the game structure.
 */
void score_bord_render (menu_t *menu, game_t * game);

/**
 * @brief Prints the score board on the display in text mode.
 * @param menu Pointer to the menu structure.
 * @param game Pointer to the game structure.
 */
void score_bord_print(menu_t *menu, game_t * game);

// RGB to 16b color
bool RBG_to_16b (color_t RGB, uint16_t *color);
bool open_img (char * file_name, unsigned char ** img);
bool save_logo (unsigned char *img, game_t *game);

#endif
/* end of gui.h */