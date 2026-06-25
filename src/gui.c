// by Pytel

#include "gui.h"
#include "driver.h"
#include "font_types.h"

#define DEBUG 1
#define TEXT_LENGHT 24

//--- GUI ---------------------------------------------------------------------

bool print_char (menu_t *menu, game_t * game, int y, int x, int index, int size, int color_index) {
	if (!game->frame_buffer) {
		fprintf(stderr, "ERROR: invalid frame buffer!\n");
		return false;
	}
	
	uint16_t *character = (uint16_t*)font_rom8x16.bits;
	uint16_t row = 0;
	int width = font_rom8x16.maxwidth;
	int height = font_rom8x16.height;
	bool pixel = 0;
	int position =-1;
	color_t RGB_color;
	uint16_t color = 0;
	int Y = 0, X = 0;
	
	for (int i = 0; i < height; i++) {
		row = character[(index*height)+i];
		//printf("%x\n", row);
		// radek
		for (int m = 0; m < size; m++) {		// row multiplyer
			for (int j = 0; j < width; j++) {
				pixel = row & ( 1 << (15-j) );
				//printf("%d\n", pixel);
				// pixely
				for (int n = 0; n < size; n++) {	// col multiplyer
					Y = y+i*size+m;
					X = x+j*size+n;
					if (Y >= HEIGHT || Y < 0) {			// valid Y?
						break;
					} else if (X >= WIDTH || X < 0) {	// valid X?
						break;
					} else {
						position = Y*WIDTH+X;
						if (pixel) {
							RGB_color.R = menu->colors[color_index].R;
							RGB_color.G = menu->colors[color_index].G;
							RGB_color.B = menu->colors[color_index].B;
							RBG_to_16b (RGB_color, &color);
							game->frame_buffer[position] = color;
						}
					}
				}
			}
		}
	}
	return true;
}
/*
char?
175 - sipka
0x61 = 97 => a
0x30 = cisla
*/
unsigned char* translate(const char *string) {
    unsigned char *translated = NULL;
    int len = 0;

    if (string && (len = strlen(string)) > 0) {
        // Alokujeme raději dostatek paměti, reálně se text zkrátí
        translated = (unsigned char*) malloc((len + 1) * sizeof(unsigned char));
    } else {
        fprintf(stderr, "ERROR, invalid string to translate!\n");
        return NULL;
    }

    int index_in = 0; // Index čtení ze vstupu
    int index_out = 0; // Index zápisu do výstupu

    while (index_in < len) {
        char c = string[index_in];

        // Standardní ASCII (latinka, čísla, mezery, interpunkce) - zapisujeme 1:1
        if ((unsigned char)c < 128) {
            translated[index_out++] = c; 
            index_in++;
        } else { // Vícebajtové UTF-8 znaky (Česká a německá abeceda)
            char ch = string[index_in + 1]; // Podíváme se na druhý bajt

            switch(c) {
                case -59: // 0xC5
                    switch (ch) {
                        case -66:  translated[index_out++] = 0x98; break; // ž
                        case -67:  translated[index_out++] = 0xa7; break; // Ž
                        case -81:  translated[index_out++] = 0x90; break; // ů
                        case -82:  translated[index_out++] = 0x9f; break; // Ů
                        case -91:  translated[index_out++] = 0x96; break; // ť
                        case -92:  translated[index_out++] = 0xa5; break; // Ť
                        case -95:  translated[index_out++] = 0x93; break; // š
                        case -96:  translated[index_out++] = 0xa2; break; // Š
                        case -103: translated[index_out++] = 0x92; break; // ř
                        case -104: translated[index_out++] = 0xa1; break; // Ř
                        case -120: translated[index_out++] = 0x97; break; // ň
                        case -121: translated[index_out++] = 0xa6; break; // Ň
                        case -72:  translated[index_out++] = 0x84; break; // Ÿ
                        default:   translated[index_out++] = '?';  break; // Neznámý znak
                    }
                    index_in += 2; // Posuneme se o oba zpracované bajty UTF-8
                    break;

                case -60: // 0xC4
                    switch (ch) {
                        case -101: translated[index_out++] = 0x91; break; // ě
                        case -102: translated[index_out++] = 0xa0; break; // Ě
                        case -113: translated[index_out++] = 0x95; break; // ď
                        case -114: translated[index_out++] = 0xa4; break; // Ď
                        case -115: translated[index_out++] = 0x94; break; // č
                        case -116: translated[index_out++] = 0xa3; break; // Č
                        default:   translated[index_out++] = '?';  break;
                    }
                    index_in += 2;
                    break;

                case -61: // 0xC3
                    switch (ch) {
                        case -67:  translated[index_out++] = 0x8d; break; // ý
                        case -70:  translated[index_out++] = 0x8f; break; // ú
                        case -77:  translated[index_out++] = 0x8e; break; // ó
                        case -83:  translated[index_out++] = 0x8c; break; // í
                        case -87:  translated[index_out++] = 0x8b; break; // é
                        case -95:  translated[index_out++] = 0x95; break; // á
                        case -99:  translated[index_out++] = 0x9c; break; // Ý
                        case -102: translated[index_out++] = 0x9e; break; // Ú
                        case -109: translated[index_out++] = 0x9d; break; // Ó
                        case -115: translated[index_out++] = 0x9b; break; // Í
                        case -119: translated[index_out++] = 0x9a; break; // É
                        case -127: translated[index_out++] = 0x99; break; // Á
                        case -65:  translated[index_out++] = 0x84; break; // ÿ
                        case -68:  translated[index_out++] = 0x86; break; // ü
                        case -74:  translated[index_out++] = 0x85; break; // ö
                        case -81:  translated[index_out++] = 0x83; break; // ï
                        case -85:  translated[index_out++] = 0x82; break; // ë
                        case -92:  translated[index_out++] = 0x81; break; // ä
                        case -100: translated[index_out++] = 0x89; break; // Ü
                        case -106: translated[index_out++] = 0x88; break; // Ö
                        case -113: translated[index_out++] = 0x83; break; // Ï
                        case -117: translated[index_out++] = 0x82; break; // Ë
                        case -124: translated[index_out++] = 0x87; break; // Ä
                        default:   translated[index_out++] = '?';  break;
                    }
                    index_in += 2;
                    break;

                default:
                    // Záchrana pro jiné neočekávané vícebajtové znaky
                    translated[index_out++] = c;
                    index_in++;
                    break;
            }
        }
    }

    // Bezpečné ukončení řetězce přesně tam, kam se reálně dokreslil
    translated[index_out] = '\0';
    return translated;
}

// NEW
void print (menu_t *menu, game_t * game, const char * string, int * y, int * x, int size, int color_index) {
	int i = 0;
	unsigned char c = 0;
	unsigned char *translated = NULL;

	if (string == NULL) return;

	bool requires_translation = false;
    for (int i = 0; string[i] != '\0'; i++) {
        if ((unsigned char)string[i] > 127) {
            requires_translation = true;
            break;
        }
    }

	if (requires_translation) {
		translated = translate(string);
	} else {
		translated = (unsigned char*) malloc((strlen(string) + 1) * sizeof(unsigned char));
		if (translated == NULL) {
			fprintf(stderr, "ERROR: cannot allocate memory for translated string!\n");
			return;
		}
		strcpy((char*)translated, string);
	}
	
	if (translated != NULL) {
		while ( (c = translated[i]) != '\0') {
			i++;
			//printf("|c: %c	ch: %d", translated[i], c);
			print_char (menu, game, *y, *x, c, size, color_index);
			*x+=8*size;
		}
		free(translated);
	}
}

char * num_to_str (int integer) {
	char * string;
	int i = 0;
	int helper = integer;
	while (helper> 0) {
		i++;
		helper /=10; 
	}
	char str[i+1];
	str[i] = '\0';
	helper = integer;
	for (int j = i-1; j >-1; j--) {
		str[j] = helper % 10+'0';
		helper /= 10;
	}
	string = str;
	//printf("string: %s\n", string);
	return string;
}

void render_logo (uint16_t * img) {
	if (!img) {
		fprintf(stderr, "ERROR: invalid img!\n");
	}
	render_gui(WIDTH, HEIGHT, img);
}

bool render_game (game_t * game) {
	render_gui(WIDTH, HEIGHT, game->game_bord);
	return true;
}

bool render_menu (menu_t *menu, game_t * game) {
    // set background
    if (!game->logo) {
        printf("ERROR: logo ptr == NULL !\n");
    }
    if ( game->frame_buffer == NULL ) {
        game->frame_buffer = (uint16_t*) malloc ( WIDTH*HEIGHT * sizeof(uint16_t) );
        if (DEBUG && game->frame_buffer) {
            printf("DEBUG: game->frame_buffer uspesne alokovana!\n");
        }
    }
    // kopirovani loga
    for (int i = 0; i < HEIGHT*WIDTH; i++) {
        game->frame_buffer[i] = game->logo[i];
    }
    
    // init
    int size = menu->size;
    int y, x;
    // base position
    if ( size == 1) {
        y = 110;    // +8
        x = 130;    // +16
    } else {
        y = 97;     // +8
        x = 50;     // +16  
    }
    int row = y;
    int col = x;
    int height = font_rom8x16.height;   // 16
    int width = font_rom8x16.maxwidth;  //  8
    
    for (int i = 0; i < NBR_ITEM; i++) {
        
        // posun textu
        if ( size > 1 && i == 0) {
            if ( menu->item > 4 ) {
                row -= 2*height*size;
            } else if (menu->item > 3 ) {
                row -= height*size;
            }
        }
        
        // vybrany prvek menu => sipka
        if ( i == menu->item && row >= 0 && row < HEIGHT) {
            print_char (menu, game, row, col, 175, size, YELLOW);
        } 
        col+=2*width*size;
        
        // OPRAVA: změněno na const char*, Row-Major 2D indexování
        const char *string = menu->item_names[i * NBR_LANGUAGE + menu->language];
        if ( row >= y) {            // text neni pod logem
            print (menu, game, string, &row, &col, size, WHITE);
        }
        // X vyber polozky za dvojteckou
        if ( i == menu->item) {
            col+=width*size;
            print_char (menu, game, row, col, 0x3a, size, GREY);
            col+=width*size;
        } else {
            col+=width*size;
            col+=width*size;
        }
        col+=width*size;
        
        // sub menu
        if ( row >= y) {            // text neni pod logem
            switch (i) {
                case LANGUAGE:
                    string = menu->languages[menu->language];
                    print (menu, game, string, &row, &col, size, WHITE);
                    break;
                case TEXT_SIZE:
                    col+=width*size;
                    print_char (menu, game, row, col, 0x30 + menu->size, size, WHITE);
                    col+=width*size;
                    break;
                case NUMBER_OF_PLAYERS:
                    col+=width*size;
                    print_char (menu, game, row, col, 0x30 + game->players, size, WHITE);
                    col+=width*size;
                    break;
                case PLAYER:
                    col+=width*size;
                    print_char (menu, game, row, col, 0x30 + menu->selected_player+1, size, WHITE);
                    col+=width*size;
                    break;
                case COLOR:
                    // OPRAVA: Indexování jmen barev na Row-Major formát podle vybraného hráče
                    if (menu->selected_player == 0) {
                        string = menu->color_names[game->player1.color_index * NBR_LANGUAGE + menu->language];
                        print (menu, game, string, &row, &col, size, game->player1.color_index);
                    } else if (menu->selected_player == 1) {
                        string = menu->color_names[game->player2.color_index * NBR_LANGUAGE + menu->language];
                        print (menu, game, string, &row, &col, size, game->player2.color_index);
                    } else if (menu->selected_player == 2) {
                        string = menu->color_names[game->player3.color_index * NBR_LANGUAGE + menu->language];
                        print (menu, game, string, &row, &col, size, game->player3.color_index);
                    }
                    break;
                case NUMBER_OF_BOTS:
                    string = num_to_str (game->bots);
                    print (menu, game, string, &row, &col, size, WHITE);
                    break;
                case DIFFICULTY:
                    string = num_to_str (menu->speed);
                    print (menu, game, string, &row, &col, size, WHITE);
                    break;
                case RESTART:
                    if (menu->restart == 1) {
                        print(menu, game, menu->words[TRUE_STR * NBR_LANGUAGE + menu->language], &row, &col, size, GREEN);
                    } else {
                        print(menu, game, menu->words[FALSE_STR * NBR_LANGUAGE + menu->language], &row, &col, size, RED);
                    }
                    break;
                case START:
                    col+=2*width*size;
                    if (menu->start == 1) {
                        print(menu, game, menu->words[TRUE_STR * NBR_LANGUAGE + menu->language], &row, &col, size, GREEN);
                    } else {
                        print(menu, game, menu->words[FALSE_STR * NBR_LANGUAGE + menu->language], &row, &col, size, RED);
                    }
                    break;
                default:
                    break;
            }
        }
        // novy radek
        row+=height*size;
        col = x;
    }
    render_gui(WIDTH, HEIGHT, game->frame_buffer);
    return true;
}

void render_score_bord (menu_t *menu, game_t * game) {
	// projde pres vsechny hrace a boty a vypise jejich score
	int size = menu->size;
	int position = 0;
    int shift = (game->bots+game->players)/2;
    int index = (game->bots+game->players) - shift;
	char *string = NULL;
	int height = font_rom8x16.height;	// 16
	int width = font_rom8x16.maxwidth;	//  8
	
	printf("Defoult posun: %d	index: %d/%d\n", position, index, game->bots+game->players);
    // display
    for (int i = 0; i < HEIGHT*WIDTH; i++) {
        game->frame_buffer[i] = game->game_bord[i];
    }
    
    int row, col;
    // base position
    if ( size == 1) {
        row = 68-(shift*height*size)/2;	// +8
        col = 160; 	// +16
    } else {
        row = 68-(shift*height*size)/2;	// +8
        col = 110; 	// +16	
    }
    int y = row;
    int x = col;
    
    // arow
    print_char (menu, game, 68+(shift*height*size)/2, x-1.5*width*size, 175, size, RED);
    
    // TODO
    // 32 LEDek
    if (index > 0) {
        LED_stripe (game->score[game->bots+game->players-index]);
    } else {
        LED_stripe (0);
    }
    
    // posun
    encoder_position(0, &menu->new_positon_w0);
    
    // posun Y
    unsigned char delta = (menu->new_positon_w0 - menu->last_positon_w0 + 255) % 255;
    if (delta < 255/2 && delta > menu->step) { 				// down
        menu->last_positon_w0 = menu->new_positon_w0;
        if ( index < game->bots+game->players ) {		
            //position+=delta;
            position+=16*size;
        }
    } else if (delta > 255/2 && 255-delta > menu->step) {	// up
        menu->last_positon_w0 = menu->new_positon_w0;
        //if ( index > 0) {
            //position-=255-delta;
            position-=16*size;
        //}
    }
    y += position; 
    index = game->bots+game->players -shift + position/(height*size);
    printf("Posun: %d	index: %d/%d\n", position, index, game->bots+game->players);
    
    if ( game->players > 0 && game->player1.enable == true ) {
        if ( y > -height*size) {			// text je mimo obraz
            print (menu, game, menu->words[PLAYERI+menu->language*NBR_WORD], &y, &x, size, game->player1.color_index);
            print (menu, game, "1", &y, &x, size, WHITE);
            print (menu, game, menu->words[SCORE+menu->language*NBR_WORD], &y, &x, size, WHITE);
            string = num_to_str (game->score[0]);
            print (menu, game, string, &y, &x, size, WHITE);
            x = col;
        }
        y+=height*size;
    }
    if ( game->players > 2 && game->player2.enable == true ) {
        if ( y > -height*size) {
            print (menu, game, menu->words[PLAYERI+menu->language*NBR_WORD], &y, &x, size, game->player2.color_index);
            print (menu, game, "2", &y, &x, size, WHITE);
            print (menu, game, menu->words[SCORE+menu->language*NBR_WORD], &y, &x, size, WHITE);
            string = num_to_str (game->score[1]);
            print (menu, game, string, &y, &x, size, WHITE);
            x = col;
        }
        y+=height*size;
    }
    if ( game->players > 3 && game->player3.enable == true ) {
        if ( y > -height*size) {
            print (menu, game, menu->words[PLAYERI+menu->language*NBR_WORD], &y, &x, size, game->player3.color_index);
            print (menu, game, "3", &y, &x, size, WHITE);
            print (menu, game, menu->words[SCORE+menu->language*NBR_WORD], &y, &x, size, WHITE);
            string = num_to_str (game->score[2]);
            print (menu, game, string, &y, &x, size, WHITE);
            x = col;
        }
        y+=height*size;
    }
    // boti
    for (int i = 0; i < game->bots; i++) {
        if ( y > -height*size) {
            print (menu, game, menu->words[BOT+menu->language*NBR_WORD], &y, &x, size, WHITE);
            string = num_to_str (i+1);
            print (menu, game, string, &y, &x, size, WHITE);
            print (menu, game, menu->words[SCORE+menu->language*NBR_WORD], &y, &x, size, WHITE);
            string = num_to_str (game->score[i+game->players]);
            print (menu, game, string, &y, &x, size, WHITE);
            x = col;
        }
        y+=height*size;
    }
    if ( y > -height*size && y < HEIGHT+height) {
        print(menu, game, menu->words[CONTINUE+menu->language*NBR_WORD], &y, &x, size, YELLOW);
    }
}

//--- Terminal ----------------------------------------------------------------
void print_menu(menu_t *menu, game_t * game) {
    if (DEBUG) {
        // Vyčistíme obrazovku terminálu před každým překreslením (volitelné, ale dělá to hezké CLI)
        // printf("\033[H\033[J"); 
        
        printf("\n=== Die Kurve : Menu ===\n\n");
        for (int i = 0; i < NBR_ITEM; i++) {
            printf("    ");
            
            // Vybraný prvek menu (kurzor)
            printf("[");
            if (i == menu->item) {
                printf("x");
            } else {
                printf(" ");
            }
            printf("]");
            
            // 1. OPRAVA: Textový popisek položky (Row-Major indexace 2D pole)
            printf(" %s", menu->item_names[i * NBR_LANGUAGE + menu->language]);
            
            // Výběr položky za dvojtečkou
            if (i == menu->item) {
                printf(" : ");
            } else {
                printf("    ");
            }
            
            // Sub-menu (hodnoty napravo)
            switch (i) {
                case LANGUAGE:
                    // Pole jazyků je jednorozměrné (flat), přístup je přímý
                    printf(" %s", menu->languages[menu->language]);
                    break;
                case TEXT_SIZE:
                    printf(" %d", menu->size);
                    break;
                case NUMBER_OF_PLAYERS:
                    printf(" %d", game->players);
                    break;
                case PLAYER:
                    printf(" %d", menu->selected_player + 1);
                    break;
                case COLOR:
                    // 2. OPRAVA: Indexování jmen barev na Row-Major formát
                    if (menu->selected_player == 0) {
                        printf(" %s", menu->color_names[game->player1.color_index * NBR_LANGUAGE + menu->language]);
                    } else if (menu->selected_player == 1) {
                        printf(" %s", menu->color_names[game->player2.color_index * NBR_LANGUAGE + menu->language]);
                    } else if (menu->selected_player == 2) {
                        printf(" %s", menu->color_names[game->player3.color_index * NBR_LANGUAGE + menu->language]);
                    }
                    break;
                case NUMBER_OF_BOTS:
                    printf(" %d", game->bots);
                    break;
                case DIFFICULTY:
                    printf(" %d", menu->speed);
                    break;
                case RESTART:
                    // 3. VYLEPŠENÍ: Propojení lokalizovaných slov TRUE_STR / FALSE_STR místo hardcoded řetězců
                    if (menu->restart) {
                        printf(" %s", menu->words[TRUE_STR * NBR_LANGUAGE + menu->language]);
                    } else {
                        printf(" %s", menu->words[FALSE_STR * NBR_LANGUAGE + menu->language]);
                    }
                    break;
                case START:
                    // 4. VYLEPŠENÍ: Propojení lokalizovaných slov i pro stav startu
                    if (menu->start) {
                        printf(" %s", menu->words[TRUE_STR * NBR_LANGUAGE + menu->language]);
                    } else {
                        printf(" %s", menu->words[FALSE_STR * NBR_LANGUAGE + menu->language]);
                    }
                    break;
                default:
                    break;
            }
            printf("\n");
        }
    }
}

void score_bord (menu_t *menu, game_t * game) {
	// terminal
	if (DEBUG) {
		// hraci
		if ( game->players > 0 && game->player1.enable == true ) {
			printf("Player: 1 score: %d\n", game->score[0]);
		}
		if ( game->players > 2 && game->player2.enable == true ) {
			printf("Player: 2 score: %d\n", game->score[1]);
		}
		if ( game->players > 3 && game->player3.enable == true ) {
			printf("Player: 3 score: %d\n", game->score[2]);
		}
		// boti
		for (int i = 0; i < game->bots; i++) {
			printf("Bot: %d score: %d\n", i, game->score[i+game->players]);
		}
	}
}

//--- image.ppm ---------------------------------------------------------------

bool RBG_to_16b (color_t RGB, uint16_t *color) {
	 uint16_t mix = 0;
	 //RED
	 mix = mix | ( (uint16_t)RGB.R << 8 & 0b1111100000000000);
	 //GREEN
	 mix = mix | ( (uint16_t)RGB.G << 3 & 0b0000011111100000);
	 //BLUE
	 mix = mix | ( (uint16_t)RGB.B >> 3 & 0b0000000000011111);
	 *color = mix;
	 return true;
}

bool open_img (char * file_name, unsigned char ** img) {
	FILE *fd;
	int Y = -1, X = -1, max_intenzita = -1, valid = false;
	char type[3];
	
	size_t n = 0;
	char *line = NULL;
	
	fd = fopen(file_name,"rb");		// cteni po Bytech
	if (!fd) {
		fprintf(stderr, "ERROR: nelze otevrit soubor: %s!\n", file_name);
		return false;
	} else if (DEBUG) {
		printf("DEBUG: soubor: %s otevren\n", file_name);
	}
	
	// Type 
	valid = fscanf(fd,"%s\n", type);
	if (valid != 1){
		fprintf(stderr, "ERORR: invalid image!\n valid: %d\n", valid);
		return false;
	}
	
	// Gimp head
	getline(&line, &n, fd);
	
	// resolution
	valid = fscanf(fd,"%d %d\n", &X, &Y);
	if (valid != 2){
		fprintf(stderr, "ERORR: invalid image!\n valid: %d\n", valid);
		return false;
	}
	if (!*img) {
		printf("DEBUG: img == NULL, alokuji pamet.\n");
		*img = (unsigned char*)malloc(Y * 3*X *sizeof(unsigned char));
	}
	if ( img == NULL){
		fprintf(stderr, "ERROR: Nepodarilo se alokovat misto!\n");
		return false;
	}
	
	// intezity
	valid = fscanf(fd,"%d\n", &max_intenzita);
	if (valid != 1){
		fprintf(stderr, "ERORR: invalid image!\n intenzity: %d\n valid: %d\n", max_intenzita, valid);
		return false;
	} else if (DEBUG) {
		printf("DEBUG: type: %s X:%d Y:%d max:%d\n", type, X, Y, max_intenzita);
	}
	
	// copy data
	valid = fread(*img, sizeof(unsigned char), Y * 3*X, fd);
	if( valid==0 ) {
		fprintf(stderr, "ERROR: Nepodarilo se skopirovat data!\n");
		return false;
	} else if (DEBUG) {
		printf("DEBUG: data uspesne nakopirovano N: %d dat!\n", valid);
	}
	fclose(fd);
	free(line);
	return true;
}

bool save_logo (unsigned char *img, game_t *game) {
	game->logo = (uint16_t*) malloc ( WIDTH*HEIGHT * sizeof(uint16_t) );
	color_t RGB_color;
	uint16_t color;
	for (int i = 0; i < WIDTH*HEIGHT; i++) {
		RGB_color.R = img[i*3+0];
		RGB_color.G = img[i*3+1];
		RGB_color.B = img[i*3+2];
		RBG_to_16b (RGB_color, &color);
		game->logo[i] = color;
	}
	return true;
}

/* end of gui.c */
