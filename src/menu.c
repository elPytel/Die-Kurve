// by Pytel

#include "menu.h"

#include "driver.h"
#include "gui.h"
#include "logger.h"

#define DEBUG 1
#define STEP 15
#define TEXT_LENGHT 24
#define FASTEST 15000	// us
#define SLOWEST 65000
#define SPEED_STEP 5000
#define MAX_BOTS 10

static const char* localization_table[NBR_ITEM][NBR_LANGUAGE] = {
    [LANGUAGE]           = {"language",          "jazyk",          "Sprache",            "jezyk"},
    [TEXT_SIZE]          = {"size of text",     "velikost textu", "Textgrösse",         "rozmiar czcionki"},
    [NUMBER_OF_PLAYERS]  = {"number of players", "počet hráčů",    "Anzahl der Spieler", "liczba graczy"},
    [PLAYER]             = {"player",            "hráč",           "Spieler",            "gracz"},
    [COLOR]              = {"color",             "barva",          "Farbe",              "kolor"},
    [NUMBER_OF_BOTS]     = {"number of bots",    "počet botů",     "Anzahl der Bots",    "liczba botów"},
    [DIFFICULTY]         = {"difficulty",        "obtížnost",      "Schwierigkeit",      "trudność"},
    [RESTART]            = {"restart",           "reset skóre",    "neu starten",        "uruchom ponownie"},
    [START]              = {"start",             "start",          "start",              "poczatek"}
};

static const char* languages_table[NBR_LANGUAGE] = {
    [EN] = "English",
    [CZ] = "Česky",
    [GE] = "Deutsche",
    [PL] = "Polskie"
};

static const char* words_table[NBR_WORD][NBR_LANGUAGE] = {
    [TRUE_STR]  = {"true",     "ano",        "Ja",         "tak"},
    [FALSE_STR] = {"false",    "ne",         "Nein",       "nie"},
    [PLAYERI]   = {"Player: ", "Hráč: ",     "Spieler: ",  "Gracz: "},
    [BOT]       = {"Bot: ",    "Bot: ",      "Bot: ",      "Nerw: "},
    [SCORE]     = {" score: ", " skóre: ",   " Ergebnis: "," wynik: "},
    [CONTINUE]  = {"Continue", "Pokračovat", "Fortsetzen", "Kontyntynuj"}
};

static const char* color_names_table[NBR_COLOR][NBR_LANGUAGE] = {
    [RED]     = {"red",     "červená",   "rot",     "czerwony"},
    [GREEN]   = {"green",   "zelená",    "grün",    "zielony"},
    [BLUE]    = {"blue",    "modrá",     "blau",    "niebieski"},
    [YELLOW]  = {"yellow",  "žlutá",     "gelb",    "zólty"},
    [MAGENTA] = {"magenta", "purpurová", "magenta", "magenta"},
    [CYAN]    = {"cyan",    "tyrkysová", "cyan",    "cyjan"},
    [GREY]    = {"grey",    "šedá",      "grau",    "szary"},
    [WHITE]   = {"white",   "bílá",      "weiss",   "bialy"}
};

// Původní RGB definice barev (tyto struktury potřebujeme modifikovatelné pro game)
static const color_t default_colors[NBR_COLOR] = {
    [RED]     = {200, 0, 0},
    [GREEN]   = {0, 200, 0},
    [BLUE]    = {0, 0, 220},
    [YELLOW]  = {200, 200, 0},
    [MAGENTA] = {200, 0, 200},
    [CYAN]    = {0, 200, 200},
    [GREY]    = {150, 150, 150},
    [WHITE]   = {250, 250, 250}
};

bool menu_init (menu_t * menu) {
    menu->step = STEP;
    menu->start = false;
    menu->restart = true;
    menu->item = 0;
    menu->selected_player = 0;
    menu->speed = (SLOWEST-FASTEST)/(2*SPEED_STEP);
    menu->size = 1;
    menu->language = CZ;
    
    // NULOVÁ ALOKACE PRO TEXTY! Pouze provážeme ukazatele na naše hotové tabulky
    menu->item_names  = (const char **)localization_table;
    menu->languages   = (const char **)languages_table;
    menu->words       = (const char **)words_table;
    menu->color_names = (const char **)color_names_table;
    
    // Alokujeme pouze dynamické pole struktur pro barvy Trona
    menu->colors = (color_t*) calloc ( NBR_COLOR, sizeof(color_t) );
    if (menu->colors == NULL) {
        fprintf(stderr, "ERROR: cannot allocate memory for colors!\n");
        return false;
    }
    
    // Zkopírujeme výchozí RGB hodnoty barev
    for (int i = 0; i < NBR_COLOR; i++) {
        menu->colors[i] = default_colors[i];
    }
    
    // Výchozí synchronizace enkodérů
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
void menu_function (menu_t * menu, game_t * game) {
    bool restart = false;
	menu->x_shift = 0;
	encoder_position(0, &menu->new_positon_w0);
	encoder_position(1, &menu->new_positon_w1);
	
	Y_menu_position(menu);
	X_menu_position(menu);
	
	// Obsluha periferií podle vybraného hráče
	if (menu->selected_player < game->active_players_count && game->score != NULL) {
		// Zjednodušená bezpečná obsluha LED pro hráče 0, 1, 2
		int color_idx = game->players[menu->selected_player].color_index;
		RGB_LED(0, menu->colors[color_idx]);
		LED_stripe(game->score[menu->selected_player]); // Ponecháno původní větvení indexů score
	}
	
	switch (menu->item) {
		case LANGUAGE:
			if (menu->x_shift == 1 && menu->language < NBR_LANGUAGE-1) {
				menu->language = menu->language + 1;
			} else if (menu->x_shift == -1 && menu->language > 0) {
				menu->language = menu->language - 1;
			}
			break;
		case TEXT_SIZE:
			if (menu->x_shift == 1 && menu->size < 2) {
				menu->size = menu->size + 1;
			} else if (menu->x_shift == -1 && menu->size > 1) {
				menu->size = menu->size - 1;
			}
			break;
		case NUMBER_OF_PLAYERS:
			if (menu->x_shift == 1 && game->active_players_count < 3) {
				game->active_players_count = game->active_players_count + 1;
				restart = true;
			} else if (menu->x_shift == -1 && game->active_players_count > 0) {
				game->active_players_count = game->active_players_count - 1;
				restart = true;
			}
			break;
		case PLAYER:
			if (menu->x_shift == 1 && menu->selected_player < game->active_players_count-1) {
				menu->selected_player = menu->selected_player + 1;
			} else if (menu->x_shift == -1 && menu->selected_player > 0) {
				menu->selected_player = menu->selected_player - 1;
			}
			break;
		case COLOR:
			choose_color(&game->players[menu->selected_player].color_index, menu->x_shift);
			break;
		case NUMBER_OF_BOTS:
			if (menu->x_shift == 1 && game->bots < MAX_BOTS) {
				game->bots = game->bots + 1;
				restart = true;
			} else if (menu->x_shift == -1 && game->bots > 0) {
				game->bots = game->bots - 1;
				restart = true;
			}
			break;
		case DIFFICULTY:
			if (menu->x_shift == 1 && (SLOWEST-(menu->speed*SPEED_STEP) > FASTEST) ) {
				menu->speed = menu->speed + 1;
			} else if (menu->x_shift == -1 && (SLOWEST-(menu->speed*SPEED_STEP) < SLOWEST) ) {
				menu->speed = menu->speed - 1;
			}
			break;
		case RESTART:
			if (menu->x_shift == 1)       menu->restart = true;
			else if (menu->x_shift == -1) menu->restart = false;
			break;
		case START:
			if (menu->x_shift) menu->start = true;
			break;
		default:
			break;
	}
	
	render_logo(game->logo);
	print_menu(menu, game);
	render_menu(menu, game);
	
	if (menu->start == true) {
		sleep(1);
	}
    menu->restart = menu->restart | restart; // proc toto????
}

bool set_game (menu_t * menu, game_t * game) {
	game->playing = true;
	game->speed = SLOWEST-(menu->speed*SPEED_STEP);
	
	// random nastaveni botu
	if ( menu->restart || game->score == NULL ) {
		game_free (game);
		game->positions = (position_t*) malloc ( game->bots*sizeof(position_t) );
		game->vectors = (position_t*) malloc ( game->bots*sizeof(position_t) );
		game->colors = (uint16_t*) malloc ( game->bots*sizeof(uint16_t) );

		for (int i = 0; i < game->bots; i++) {		// random barvicky
			color_t color;
			color.R = (rand() % 200) + 50;	// 255, 200 + 55, 155 + 100
			color.G = (rand() % 200) + 50;
			color.B = (rand() % 200) + 50;
			RBG_to_16b(color, &game->colors[i]);
		}
		
		game->live_bots = (bool*) malloc ( game->bots*sizeof(bool) );
		game->directions = (int*) malloc ( (game->bots) * sizeof(int) );
		game->score = (int*) malloc ( (game->bots+game->active_players_count) * sizeof(int) );
		for (int i = 0; i < game->bots+game->active_players_count; i++) {
			game->score[i] = 0;
		}
		
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
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (i < game->active_players_count) {
			game->players[i].enable = true;
			game->players[i].alive = true;
		} else {
			game->players[i].enable = false;
			game->players[i].alive = false;
		}
		RBG_to_16b (menu->colors[game->players[i].color_index], &game->players[i].color);
		game->players[i].position.x = 5 + (rand() % (WIDTH-10) );
		game->players[i].position.y = 5 + (rand() % (HEIGHT-10) );
		game->players[i].vector.x = 0;
		game->players[i].vector.y = 0;
		if (DEBUG) {
			logger_log("Player position: (%d, %d)", game->players[i].position.x, game->players[i].position.y);
			logger_log("Player color index: %d", game->players[i].color_index);
			unsigned char R,G,B;
			R = menu->colors[game->players[i].color_index].R;
			G = menu->colors[game->players[i].color_index].G;
			B = menu->colors[game->players[i].color_index].B;
			logger_log("R: %d G: %d B: %d	", R, G, B);
			logger_log("16b color: %d", game->players[i].color);
		}
	}
	return true;
}

void kill_menu (menu_t * menu) {
    if (menu->colors) {
        free(menu->colors);
        menu->colors = NULL;
    }
    // Všechny textové ukazatele jen bezpečně odpojíme
    menu->item_names = NULL;
    menu->languages = NULL;
    menu->words = NULL;
    menu->color_names = NULL;
}

/* end of menu.c */
