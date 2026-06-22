// by Pytel

#include "menu.h"

#include "driver.h"
#include "gui.h"

#define DEBUG 1
#define STEP 15
#define TEXT_LENGHT 24
#define FASTEST 15000	// us
#define SLOWEST 65000
#define SPEED_STEP 5000
#define MAX_BOTS 10

bool menu_init (menu_t * menu) {
	menu->last_positon_w0 = -1;
	menu->last_positon_w1 = -1;
	menu->step = STEP;
	menu->start = false;
	menu->restart = true;
	menu->item = 0;
	menu->selected_player = 0;
	menu->speed = (SLOWEST-FASTEST)/(2*SPEED_STEP);
	menu->size = 1;
	
	// alokace pameti polozek
	menu->item_names = NULL;
	menu->item_names = (char**) calloc ( NBR_ITEM*NBR_LANGUAGE, sizeof(char*) );
	for (int i = 0; i < NBR_ITEM*NBR_LANGUAGE; i++) {
		menu->item_names[i] = (char*) calloc ( TEXT_LENGHT, sizeof(char) );
	}
	if (menu->item_names == NULL) {
		fprintf(stderr, "ERROR: cannot alocate memory!\n");
	}
	
	// definovani jmeneho prosturu Menu
	for (int i = 0; i < NBR_ITEM; i++) {
		switch (i) {
			case LANGUAGE:
				menu->item_names[i+EN*NBR_ITEM] = "language\0";
				menu->item_names[i+CZ*NBR_ITEM] = "jazyk\0";
				menu->item_names[i+GE*NBR_ITEM] = "Sprache\0";
				menu->item_names[i+PL*NBR_ITEM] = "jezyk\0";
				break;
			case TEXT_SIZE:
				menu->item_names[i+EN*NBR_ITEM] = "size of text\0";
				menu->item_names[i+CZ*NBR_ITEM] = "velikost textu\0";
				menu->item_names[i+GE*NBR_ITEM] = "Textgrösse\0";
				menu->item_names[i+PL*NBR_ITEM] = "rozmiar czcionki\0";
				break;
			case NUMBER_OF_PLAYERS:
				menu->item_names[i+EN*NBR_ITEM] = "number of players\0";
				menu->item_names[i+CZ*NBR_ITEM] = "počet hráčů\0";
				menu->item_names[i+GE*NBR_ITEM] = "Anzahl der Spieler\0";
				menu->item_names[i+PL*NBR_ITEM] = "liczba graczy\0";
				break;
			case PLAYER:
				menu->item_names[i+EN*NBR_ITEM] = "player\0";
				menu->item_names[i+CZ*NBR_ITEM] = "hráč\0";
				menu->item_names[i+GE*NBR_ITEM] = "Spieler\0";
				menu->item_names[i+PL*NBR_ITEM] = "gracz\0";
				break;
			case COLOR:
				menu->item_names[i+EN*NBR_ITEM] = "color\0";
				menu->item_names[i+CZ*NBR_ITEM] = "barva\0";
				menu->item_names[i+GE*NBR_ITEM] = "Farbe\0";
				menu->item_names[i+PL*NBR_ITEM] = "kolor\0";
				break;
			case NUMBER_OF_BOTS:
				menu->item_names[i+EN*NBR_ITEM] = "number of shoes\0";
				menu->item_names[i+CZ*NBR_ITEM] = "počet botů\0";
				menu->item_names[i+GE*NBR_ITEM] = "Anzahl der Schuhe\0";
				menu->item_names[i+PL*NBR_ITEM] = "liczba butów\0";
				break;
			case DIFFICULTY:
				menu->item_names[i+EN*NBR_ITEM] = "difficulty\0";
				menu->item_names[i+CZ*NBR_ITEM] = "obtížnost\0";
				menu->item_names[i+GE*NBR_ITEM] = "Schwierigkeit\0";
				menu->item_names[i+PL*NBR_ITEM] = "trudnošč\0";
				break;
			case RESTART:
				menu->item_names[i+EN*NBR_ITEM] = "restart\0";
				menu->item_names[i+CZ*NBR_ITEM] = "reset skóre\0";
				menu->item_names[i+GE*NBR_ITEM] = "neu starten\0";
				menu->item_names[i+PL*NBR_ITEM] = "uruchom ponownie\0";
				break;
			case START:
				menu->item_names[i+EN*NBR_ITEM] = "start\0";
				menu->item_names[i+CZ*NBR_ITEM] = "start\0";
				menu->item_names[i+GE*NBR_ITEM] = "start\0";
				menu->item_names[i+PL*NBR_ITEM] = "poczatek\0";
				break;
			
			default:
				break;
		}	
	}
	
	menu->language = CZ;
	// alokace pameti jazyku
	menu->languages = NULL;
	menu->languages = (char**) calloc ( NBR_LANGUAGE, sizeof(char*) );
	for (int i = 0; i < NBR_LANGUAGE; i++) {
		menu->languages[i] = (char*) calloc ( TEXT_LENGHT, sizeof(char) );
	}
	
	// definovani jmeneho prosturu
	for (int i = 0; i < NBR_LANGUAGE; i++) {
		switch (i) {
			case EN:
				menu->languages[i] = "English\0";
				break;
			case CZ:
				menu->languages[i] = "Česky\0";
				break;
			case GE:
				menu->languages[i] = "Deutsche\0";
				break;
			case PL:
				menu->languages[i] = "Polskie\0";
				break;
			
			default:
				break;
		}
	}
	
	
	// alokace pameti slov
	menu->words = NULL;
	menu->words = (char**) malloc ( NBR_WORD*NBR_LANGUAGE * sizeof(char*) );
	for (int i = 0; i < NBR_WORD*NBR_LANGUAGE; i++) {
		menu->words[i] = (char*) calloc ( TEXT_LENGHT, sizeof(char) );
	}
	
	// definovani jmeneho prosturu
	for (int i = 0; i < NBR_WORD; i++) {
		switch (i) {
			case TRUE:
				menu->words[i+EN*NBR_WORD] = "true\0";
				menu->words[i+CZ*NBR_WORD] = "ano\0";
				menu->words[i+GE*NBR_WORD] = "Ja\0";
				menu->words[i+PL*NBR_WORD] = "tak\0";
				break;
			case FALSE:
				menu->words[i+EN*NBR_WORD] = "false\0";
				menu->words[i+CZ*NBR_WORD] = "ne\0";
				menu->words[i+GE*NBR_WORD] = "Nein\0";
				menu->words[i+PL*NBR_WORD] = "nie\0";
				break;
			case PLAYERI:
				menu->words[i+EN*NBR_WORD] = "Player: \0";
				menu->words[i+CZ*NBR_WORD] = "Hráč: \0";
				menu->words[i+GE*NBR_WORD] = "Spieler: \0";
				menu->words[i+PL*NBR_WORD] = "Gracz: \0";
				break;
			case BOT:
				menu->words[i+EN*NBR_WORD] = "Bot: \0";
				menu->words[i+CZ*NBR_WORD] = "Bot: \0";
				menu->words[i+GE*NBR_WORD] = "Bot: \0";
				menu->words[i+PL*NBR_WORD] = "Nerw: \0";
				break;
			case SCORE:
				menu->words[i+EN*NBR_WORD] = " score: \0";
				menu->words[i+CZ*NBR_WORD] = " skóre: \0";
				menu->words[i+GE*NBR_WORD] = " Ergebnis: \0";
				menu->words[i+PL*NBR_WORD] = " wynik: \0";
				break;
			case CONTINUE:
				menu->words[i+EN*NBR_WORD] = "  Continue\0";
				menu->words[i+CZ*NBR_WORD] = "  Pokračovat\0";
				menu->words[i+GE*NBR_WORD] = "  Fortsetzen\0";
				menu->words[i+PL*NBR_WORD] = "  Kontyntynuj\0";
				break;
			
			default:
				break;
		}	
	}
	
	// alokace pameti jmen barev
	menu->color_names = NULL;
	menu->color_names = (char**) calloc ( NBR_COLOR*NBR_LANGUAGE, sizeof(char*) );
	for (int i = 0; i < NBR_COLOR*NBR_LANGUAGE; i++) {
		menu->color_names[i] = (char*) calloc ( TEXT_LENGHT, sizeof(char) );
	}
	if (menu->color_names == NULL) {
		fprintf(stderr, "ERROR: cannot alocate memory!\n");
	}
	// alokace pameti dat barev
	menu->colors = NULL;
	menu->colors = (color_t*) calloc ( NBR_COLOR, sizeof(color_t) );
	if (menu->colors == NULL) {
		fprintf(stderr, "ERROR: cannot alocate memory!\n");
	}
	// definovani jmeneho prosturu colors
	for (int i = 0; i < NBR_COLOR; i++) {
		switch (i) {
			case RED:
				menu->color_names[i+EN*NBR_COLOR] = "red\0";
				menu->color_names[i+CZ*NBR_COLOR] = "červená\0";
				menu->color_names[i+GE*NBR_COLOR] = "rot\0";
				menu->color_names[i+PL*NBR_COLOR] = "czerwony\0";
				menu->colors[i].R = 200;
				menu->colors[i].G = 0;
				menu->colors[i].B = 0;
				break;
			case GREEN:
				menu->color_names[i+EN*NBR_COLOR] = "green\0";
				menu->color_names[i+CZ*NBR_COLOR] = "zelená\0";
				menu->color_names[i+GE*NBR_COLOR] = "grün\0";
				menu->color_names[i+PL*NBR_COLOR] = "zielony\0";
				menu->colors[i].R = 0;
				menu->colors[i].G = 200;
				menu->colors[i].B = 0;
				break;
			case BLUE:
				menu->color_names[i+EN*NBR_COLOR] = "blue\0";
				menu->color_names[i+CZ*NBR_COLOR] = "modrá\0";
				menu->color_names[i+GE*NBR_COLOR] = "blau\0";
				menu->color_names[i+PL*NBR_COLOR] = "niebieski\0";
				menu->colors[i].R = 0;
				menu->colors[i].G = 0;
				menu->colors[i].B = 220;
				break;
			case YELLOW:
				menu->color_names[i+EN*NBR_COLOR] = "yellow\0";
				menu->color_names[i+CZ*NBR_COLOR] = "žlutá\0";
				menu->color_names[i+GE*NBR_COLOR] = "gelb\0";
				menu->color_names[i+PL*NBR_COLOR] = "zólty\0";
				menu->colors[i].R = 200;
				menu->colors[i].G = 200;
				menu->colors[i].B = 0;
				break;
			case MAGENTA:
				menu->color_names[i+EN*NBR_COLOR] = "magenta\0";
				menu->color_names[i+CZ*NBR_COLOR] = "purpurová\0";
				menu->color_names[i+GE*NBR_COLOR] = "magenta\0";
				menu->color_names[i+PL*NBR_COLOR] = "magenta\0";
				menu->colors[i].R = 200;
				menu->colors[i].G = 0;
				menu->colors[i].B = 200;
				break;
			case CYAN:
				menu->color_names[i+EN*NBR_COLOR] = "cyan\0";
				menu->color_names[i+CZ*NBR_COLOR] = "tyrkysová\0";
				menu->color_names[i+GE*NBR_COLOR] = "cyan\0";
				menu->color_names[i+PL*NBR_COLOR] = "cyjan\0";
				menu->colors[i].R = 0;
				menu->colors[i].G = 200;
				menu->colors[i].B = 200;
				break;
			case GREY:
				menu->color_names[i+EN*NBR_COLOR] = "grey\0";
				menu->color_names[i+CZ*NBR_COLOR] = "šedá\0";
				menu->color_names[i+GE*NBR_COLOR] = "grau\0";
				menu->color_names[i+PL*NBR_COLOR] = "szary\0";
				menu->colors[i].R = 150;
				menu->colors[i].G = 150;
				menu->colors[i].B = 150;
				break;
			case WHITE:
				menu->color_names[i+EN*NBR_COLOR] = "white\0";
				menu->color_names[i+CZ*NBR_COLOR] = "bílá\0";
				menu->color_names[i+GE*NBR_COLOR] = "weiss\0";
				menu->color_names[i+PL*NBR_COLOR] = "bialy\0";
				menu->colors[i].R = 250;
				menu->colors[i].G = 250;
				menu->colors[i].B = 250;
				break;
			
			default:
				break;
		}	
	}
	
	// TODO
	// nutne pro demo
	menu->last_positon_w0 = 0;
	menu->new_positon_w0 = 0;
	menu->last_positon_w1 = 0;
	menu->new_positon_w1 = 0;
	
	return true;
}

void choose_color (int *index, int x_shift) {
	if (x_shift == 1 && *index < NBR_COLOR-1) {
		*index = *index +1;
	} else if (x_shift == -1 && *index > 0) {
		*index = *index -1;
	} else {
	 	*index = *index;
	}
}

void Y_menu_position (menu_t * menu) {
	//printf("From: %d 	to: %d\n",  menu->last_positon_w0, menu->new_positon_w0);
	unsigned char delta = (menu->new_positon_w0 - menu->last_positon_w0 + 255) % 255;
	if (delta < 255/2 && delta > menu->step) { 			// forward
		menu->last_positon_w0 = menu->new_positon_w0;
		if ( menu->item < NBR_ITEM-1 ) {		
			menu->item = menu->item +1;
		}
	} else if (delta > 255/2 && 255-delta > menu->step) {	// backward
		menu->last_positon_w0 = menu->new_positon_w0;
		if ( menu->item > 0) {
			menu->item--;
		}
	}
}

void X_menu_position (menu_t * menu) {
	unsigned char delta = (menu->new_positon_w1 - menu->last_positon_w1 + 255) % 255;
	if (delta < 255/2 && delta > menu->step) { 				//right
		menu->last_positon_w1 = menu->new_positon_w1;
		menu->x_shift = 1;
	} else if (delta > 255/2 && 255-delta > menu->step) {	// left
		menu->last_positon_w1 = menu->new_positon_w1;
		menu->x_shift = -1;
	}
}

// vytiskne menu a zepta se na nastaveni novych hernich paramteru
void menu_function (menu_t * menu, game_t * game) {
	// inicializace
	bool restart = false;
	menu->start = false;
	menu->item = 0;
	// nacteni pocatecni pozice hursoru Y
	wheel_position(0, &menu->last_positon_w0);
	// nacteni pocatecni pozice hursoru X
	wheel_position(1, &menu->last_positon_w1);
	
	while (!menu->start) {
		// aktualizace
		menu->x_shift = 0;
		wheel_position(0, &menu->new_positon_w0);
		wheel_position(1, &menu->new_positon_w1);
		
		// posun Y
		Y_menu_position(menu);
		
		// posun X
		X_menu_position(menu);
		
		// vystup
		if (menu->selected_player == 0) {
			// RGB LED
			RGB_LED (0, menu->colors[game->player1.color_index]);
			// LED stripe
			if (game->score != NULL) {
				LED_stripe (game->score[0]);
			}
		} else if (menu->selected_player == 1) {
			// RGB LED
			RGB_LED (0, menu->colors[game->player2.color_index]);
			// LED stripe
			if (game->score != NULL) {
				LED_stripe (game->score[2]);
			}
		} else if (menu->selected_player == 2) {
			// RGB LED
			RGB_LED (0, menu->colors[game->player3.color_index]);
			// LED stripe
			if (game->score != NULL) {
				LED_stripe (game->score[2]);
			}
		}
		
		// menu
		switch (menu->item) {
			case LANGUAGE:
				if (menu->x_shift == 1 && menu->language < NBR_LANGUAGE-1) {
					menu->language = menu->language +1;
				} else if (menu->x_shift == -1 && menu->language > 0) {
					menu->language = menu->language -1;
				}
				break;
			case TEXT_SIZE:
				if (menu->x_shift == 1 && menu->size < 2) {
					menu->size = menu->size +1;
				} else if (menu->x_shift == -1 && menu->size > 1) {
					menu->size = menu->size -1;
				}
				break;
			case NUMBER_OF_PLAYERS:
				if (menu->x_shift == 1 && game->players < 3) {		// magic number
					game->players = game->players +1;
					restart = true;
				} else if (menu->x_shift == -1 && game->players > 0) {
					game->players = game->players -1;
					restart = true;
				}
				break;
			case PLAYER:
				if (menu->x_shift == 1 && menu->selected_player < game->players-1) {
					menu->selected_player = menu->selected_player +1;
				} else if (menu->x_shift == -1 && menu->selected_player > 0) {
					menu->selected_player = menu->selected_player -1;
				}
				break;
			case COLOR:
				if (menu->selected_player == 0) {
					choose_color(&game->player1.color_index, menu->x_shift);
					if (DEBUG) {
						printf("Color index: %d\n", game->player1.color_index);
						unsigned char R,G,B;
						R = menu->colors[game->player1.color_index].R;
						G = menu->colors[game->player1.color_index].G;
						B = menu->colors[game->player1.color_index].B;
						printf("R: %d G: %d B: %d\n", R, G, B);
					}
				} else if (menu->selected_player == 1) {
					choose_color(&game->player2.color_index, menu->x_shift);
				} else if (menu->selected_player == 2) {
					choose_color(&game->player3.color_index, menu->x_shift);
				}
				break;
			case NUMBER_OF_BOTS:
				if (menu->x_shift == 1 && game->bots < MAX_BOTS) {
					game->bots = game->bots +1;
					restart = true;
				} else if (menu->x_shift == -1 && game->bots > 0) {
					game->bots = game->bots -1;
					restart = true;
				}
				break;
			case DIFFICULTY:
				if (menu->x_shift == 1 && (SLOWEST-(menu->speed*SPEED_STEP) > FASTEST) ) {
					menu->speed = menu->speed +1;
				} else if (menu->x_shift == -1 && (SLOWEST-(menu->speed*SPEED_STEP) < SLOWEST) ) {
					menu->speed = menu->speed -1;
				}
				break;
			case RESTART:
				if ( menu->x_shift == 1) {
					menu->restart = true;
				} else if ( menu->x_shift == -1 ) {
					menu->restart = false;
				} 
				break;
			case START:		// start hry
				if ( menu->x_shift ) {
					menu->start = true;
				}
				break;
			default:
				break;
		}
		
		// vypis menu
		print_menu(menu, game);
		render_menu(menu, game);
		
		if (menu->start == true) {
			sleep(1);
		}
	}
	printf(" DEBUG: menu->restart = %d	restart = %d\n", menu->restart, restart);
	menu->restart = menu->restart | restart;
	
}

bool set_game (menu_t * menu, game_t * game) {
	game->playing = true;
	game->speed = SLOWEST-(menu->speed*SPEED_STEP);
	
	// random nastaveni botu
	if ( menu->restart || game->score == NULL ) {
		free_game (game);
		// pozice
		game->positions = (position_t*) malloc ( game->bots*sizeof(position_t) );
		
		// vektor
		game->vectors = (position_t*) malloc ( game->bots*sizeof(position_t) );
		
		// alokace barev botu
		game->colors = (uint16_t*) malloc ( game->bots*sizeof(uint16_t) );
		for (int i = 0; i < game->bots; i++) {		// random barvicky
			color_t color;
			color.R = (rand() % 200) + 50;	// 255, 200 + 55, 155 + 100
			color.G = (rand() % 200) + 50;
			color.B = (rand() % 200) + 50;
			RBG_to_16b(color, &game->colors[i]);
		}
		
		// zivoty
		game->live_bots = (bool*) malloc ( game->bots*sizeof(bool) );
		
		// smery
		game->directions = (int*) malloc ( (game->bots) * sizeof(int) );
		
		// alokace score a vymazani skore
		game->score = (int*) malloc ( (game->bots+game->players) * sizeof(int) );
		for (int i = 0; i < game->bots+game->players; i++) {
			game->score[i] = 0;
		}
		
		// mezery
		game->spaces = (int*) malloc ( game->bots*sizeof(int) );
		
	}
	
	// oziveni mrtvych botu
	for (int i = 0; i < game->bots; i++) {
		game->live_bots[i] = true;
	}
	
	// mezery
	for (int i = 0; i < game->bots; i++) {
		game->spaces[i] = 0;
	}
	
	// herni pole
	if ( game->game_bord == NULL ) {
		game->game_bord = (uint16_t*) malloc ( WIDTH*HEIGHT * sizeof(uint16_t) );
	}
	
	// vyresetuje herni pole
	for (int i = 0; i < WIDTH*HEIGHT; i++) {
		game->game_bord[i] = 0;
	}
	
	// frame buffer
	if ( game->frame_buffer == NULL ) {
		game->frame_buffer = (uint16_t*) malloc ( WIDTH*HEIGHT * sizeof(uint16_t) );
	}
	
	// logo
	if ( game->logo == NULL ) {
		game->logo = (uint16_t*) malloc ( WIDTH*HEIGHT * sizeof(uint16_t) );
	}
	
	// random pozice botu
	for (int i = 0; i < game->bots; i++) {
		game->positions[i].x = 5 + (rand() % (WIDTH-10) );
		game->positions[i].y = 5 + (rand() % (HEIGHT-10) );
	}
	
	// random vektor botu
	for (int i = 0; i < game->bots; i++) {
		game->vectors[i].x = 0;
		game->vectors[i].y = 0;
		while (game->vectors[i].y == 0 && game->vectors[i].x == 0 ) {
			game->vectors[i].x = rand()%3 -1;
			game->vectors[i].y = rand()%3 -1;
		}
	}
	
	// aktivace hracu
	if ( game->players > 0 ) {
		game->player1.enable = true;
		game->player1.alive = true;
		// nastaveni barvy
		RBG_to_16b (menu->colors[game->player1.color_index], &game->player1.color);
		if (DEBUG) {
			printf("Player color index: %d\n", game->player1.color_index);
			unsigned char R,G,B;
			R = menu->colors[game->player1.color_index].R;
			G = menu->colors[game->player1.color_index].G;
			B = menu->colors[game->player1.color_index].B;
			printf("R: %d G: %d B: %d	", R, G, B);
			printf("16b color: %d\n", game->player1.color);
		}
		// nastaveni pocatecni pozice
		game->player1.position.x = 5 + (rand() % (WIDTH-10) );	// +-5 od kazdeho okraje = magic number
		game->player1.position.y = 5 + (rand() % (HEIGHT-10) );
	}
	if ( game->players > 2 ) {
		game->player2.enable = true;
		game->player2.alive = true;
		// nastaveni barvy
		RBG_to_16b (menu->colors[game->player2.color_index], &game->player2.color);
		// nastaveni pocatecni pozice
		game->player2.position.x = 5 + (rand() % (WIDTH-10) );
		game->player2.position.y = 5 + (rand() % (HEIGHT-10) );
	}
	if ( game->players > 3 ) {
		game->player3.enable = true;
		game->player3.alive = true;
		// nastaveni barvy
		RBG_to_16b (menu->colors[game->player2.color_index], &game->player2.color);
		// nastaveni pocatecni pozice
		game->player3.position.x = 5 + (rand() % (WIDTH-10) );
		game->player3.position.y = 5 + (rand() % (HEIGHT-10) );
	}
	
	return true;
}

void kill_menu (menu_t * menu) {
	if (menu->item_names) {
		for (int i = 0; i < NBR_ITEM*NBR_LANGUAGE; i++) {
			free (menu->item_names[i]);
		}
		free (menu->item_names);
		menu->item_names = NULL;
	}
	if (menu->languages) {
		for (int i = 0; i < NBR_LANGUAGE; i++) {
			free (menu->languages[i]);
		}
		free(menu->languages);
		menu->languages = NULL;
	}
	if (menu->words) {
		for (int i = 0; i < NBR_WORD*NBR_LANGUAGE; i++) {
			free (menu->item_names[i]);
		}
		free(menu->words);
		menu->words = NULL;
	}
	if (menu->color_names) {
		for (int i = 0; i < NBR_COLOR*NBR_LANGUAGE; i++) {
			free (menu->color_names[i]);
		}
		free (menu->item_names);
		menu->item_names = NULL;
	}
	if (menu->colors) {
		free (menu->colors);
		menu->item_names = NULL;
	}
}

/*
void Y_menu_position (menu_t * menu) {
	if ( menu->last_positon_w0 - menu->new_positon_w0 > menu->step) {			// backward
		// TODO - prekroceni pres 360°
		menu->last_positon_w0 = menu->new_positon_w0;
		if ( menu->item > 0) {
			menu->item--;
		}
	} else if ( menu->new_positon_w0 - menu->last_positon_w0 > menu->step) {	// forward
		menu->last_positon_w0 = menu->new_positon_w0;
		if ( menu->item < NBR_ITEM-1 ) {
			menu->item = menu->item +1;
		}
	}
}

void X_menu_position (menu_t * menu) {
	if ( menu->last_positon_w1 - menu->new_positon_w1 > menu->step) {			// left
		menu->last_positon_w1 = menu->new_positon_w1;
		menu->x_shift = -1;
	} else if ( menu->new_positon_w1 - menu->last_positon_w1 > menu->step) {	//right
		menu->last_positon_w1 = menu->new_positon_w1;
		menu->x_shift = 1;
	}
}

void Y_menu_position (menu_t * menu) {
	printf("From: %d 	to: %d\n",  menu->last_positon_w0, menu->new_positon_w0);
	if ( (menu->last_positon_w0 + menu->step) % 360 > menu->new_positon_w0 ) {	// backward
		menu->last_positon_w0 = menu->new_positon_w0;
		if ( menu->item > 0) {
			menu->item--;
		}
	} else if ( (menu->last_positon_w0 + menu->step) % 360 < menu->new_positon_w0 ) {	// forward
		menu->last_positon_w0 = menu->new_positon_w0;
		if ( menu->item < NBR_ITEM-1 ) {
			menu->item = menu->item +1;
		}
	}
}
*/

/* end of menu.c */
