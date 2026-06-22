// by Pytel

#ifndef __DRIVER_h__
#define __DRIVER_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "game.h"

/* Nastaveni otocneho volice
 * number: vyber volice 0-2
 * navratova hodnota degree, je uhel natoceni
 */
bool wheel_position(int number, uint8_t *degree);	// , bool *press

/* Rozsvti RGB LEDku na na požadovanou barvu
 */
bool RGB_LED (int number, color_t color);

// 32 LEDek v rade
void LED_stripe (int number);

void Call_termios(int reset);


#include "xwin_sdl.h"

# define HEIGHT 320
# define WIDTH 	480

	//240	//480
	//320	//640

bool gui_init ();
bool kill_gui ();
bool render_gui (int w, int h, uint16_t *img);

#endif
/* end of driver.h */