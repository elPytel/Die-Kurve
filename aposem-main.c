/*
 * File name: aposem-main.c
 * Date:      2020/06/02
 * Author:    Jaroslav Körner, Kateřina Poláková
 * Hra pro APO desku - Achtung, die Kurve!
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "game.h"
#include "menu.h"
#include "gui.h"
#include "driver.h"

#define DEBUG 1

#define OK 1
#define NOK -1

// - main ---------------------------------------------------------------------

int main () {
	// inicializace promenych
	time_t start_time;
	char *logo_file = "dieKurve.ppm";
	
	gui_init ();	// alokoje a smaze graficky buffer
	game_t game;
	game_init(&game);
	
	menu_t menu;
	menu_init(&menu);
	
	if (DEBUG) {	//0 && 
		game.bots = 0;
		menu.size = 1;
		menu.speed = 10;
	}
	
	// nacteni herniho logo
	unsigned char *img = NULL;
	open_img(logo_file, &img);
	save_logo(img, &game);
	free(img);
	
	// nekonecna smycka ve ktere stale beha program
	while (true != false) {
		// menu
		render_logo(game.logo);
		menu_function(&menu, &game);
		
		// nastaveni hernich parametru podle dat z menu
		set_game (&menu, &game);
		
		// herni smycka
		while ( playing(&game) ) {
			if (DEBUG) {
				printf("New turn.\n");
			}
			
			time(&start_time);
			
			// aktualizace vektrou
			// AI
			AI_move(&game);
			// hrac
			player_move(&game);
			
			// vyhodnoceni tahu
			// AI i hrace
			valid_play(&game);
			
			// vykresleni obrazu
			render_game(&game);
			
			// casovani
			//sleep(1);
			printf(" Start time: %ld\n", start_time);				// vraci hodnotu pouze v sekundach, najit lepsi jemnejsi zpusob
			printf(" Executed in: %ld sec\n", time(NULL)-start_time);
			usleep( game.speed - (time(NULL)-start_time) );
			//usleep(50000);
		}
		// vypsani skore po skonceni hry
		score_bord(&menu, &game);
		render_score_bord(&menu, &game);
		
		if ( getchar() == 'q' ) {
			break;
		}
		if (DEBUG) {
			menu.restart = false;
		}
	}
	kill_gui();
	kill_menu (&menu);
	kill_game_all (&game);
	return OK;
}

/*
*/

/* end of aposem-main.c */
