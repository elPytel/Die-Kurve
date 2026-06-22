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
unsigned char* translate(char *string) {
	unsigned char *translated = NULL;
	int len = 0;
	// ASERT
	if ( string && ( len = strlen(string) )>0 ) {
		translated = (unsigned char*) malloc ( (len+1) *sizeof(unsigned char) );
		//printf("Aloc = %ld\n", strlen(string)+1);
	} else {
		printf("ERROR, invalid string to translate!\n");
		return NULL;
	}
	// inicializace
	char c = 0;
	char ch = 0;
	int index1 = -1;
	int index2 = -1;
	//printf(" index1 = %d < strlen(string) = %d ?", index1, len);
	while ( index1 < len ) {	//(int)strlen(string)
		index1++;
		index2++;
		//printf("\n index1 = %d	index2 = %d	", index1, index2);
		// latinka
		c = string[index1];
		if ( ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == ' ' || ('0' <= c && c <= '9') ) {
			//printf(" c = %c	", c);
			translated[index2] = 97+c-'a';
		} else {	// ceska abeceda
			//printf(" ch = %d	", c);
			switch(c) {
				case -59:
					index1++;
					ch = string[index1];
					//printf(" ch = %d	", ch);
					switch (ch) {
						case -66:	// ž
							translated[index2] = 0x98;
							break;
						case -67:	// Ž
							translated[index2] = 0xa7;
							break;
						case -81:	// ů
							translated[index2] = 0x90;
							break;
						case -82:	// Ů
							translated[index2] = 0x9f;
							break;
						case -91:	// ť
							translated[index2] = 0x96;
							break;
						case -92:	// Ť
							translated[index2] = 0xa5;
							break;
						case -95:	// š
							translated[index2] = 0x93;
							//printf(" c = š	");
							break;
						case -96:	// Š
							translated[index2] = 0xa2;
							break;
						case -103:	// ř
							translated[index2] = 0x92;
							break;
						case -104:	// Ř
							translated[index2] = 0xa1;
							break;
						case -120:	// ň
							translated[index2] = 0x97;
							break;
						case -121:	// Ň
							translated[index2] = 0xa6;
							break;
						// nemecka psismenka
						case -72:	// Ÿ
							translated[index2] = 0x84;		// je pouze male
							break;
						default:
							break;
					} // swtich2 end
					break;
				case -60:
					index1++;
					ch = string[index1];
					switch (ch) {
						case -101:	// ě
							translated[index2] = 0x91;
							break;
						case -102:	// Ě
							translated[index2] = 0xa0;
							break;
						case -113:	// ď
							translated[index2] = 0x95;
							break;
						case -114:	// Ď
							translated[index2] = 0xa4;
							break;
						case -115:	// č
							translated[index2] = 0x94;
							break;
						case -116:	// Č
							translated[index2] = 0xa3;
							break;
						default:
							break;
					} // swtich2 end
					break;
				case -61:
					index1++;
					ch = string[index1];
					//printf(" ch = %d	", ch);
					switch (ch) {
						case -67:	// ý
							translated[index2] = 0x8d;
							break;
						case -70:	// ú
							translated[index2] = 0x8f;
							break;
						case -77:	// ó
							translated[index2] = 0x8e;
							//printf(" c = ó	");
							break;
						case -83:	// í
							translated[index2] = 0x8c;
							break;
						case -87:	// é
							translated[index2] = 0x8b;
							break;
						case -95:	// á
							translated[index2] = 0x8a;
							break;
						case -99:	// Ý
							translated[index2] = 0x9c;
							break;
						case -102:	// Ú
							translated[index2] = 0x9e;
							break;
						case -109:	// Ó
							translated[index2] = 0x9d;
							break;
						case -115:	// Í
							translated[index2] = 0x9b;
							break;
						case -119:	// É
							translated[index2] = 0x9a;
							break;
						case -127:	// Á
							translated[index2] = 0x99;
							break;
						// nemecka psismenka	
						case -65:	// ÿ
							translated[index2] = 0x84;
							break;
						case -68:	// ü
							translated[index2] = 0x86;
							break;
						case -74:	// ö
							translated[index2] = 0x85;
							break;
						case -81:	// ï
							translated[index2] = 0x83;
							break;
						case -85:	// ë
							translated[index2] = 0x82;
							break;
						case -92:	// ä
							translated[index2] = 0x81;
							break;
						case -100:	// Ü
							translated[index2] = 0x89;
							break;
						case -106:	// Ö
							translated[index2] = 0x88;
							break;
						case -113:	// Ï
							translated[index2] = 0x83;		// je pouze male
							break;
						case -117:	// Ë
							translated[index2] = 0x82;		// je pouze male
							break;
						case -124:	// Ä
							translated[index2] = 0x87;
							break;
						default:
							break;
					} // swtich2 end
					break;
				default:
					translated[index2] = c;
					break;
			} // switch1 end
		} // else end
	}
	
	// ukonceni
	if ( index2 < len ) {
		index2++;
		translated[index2] = '\0';
	} else {
		translated[index2] = '\0';
	}
	
	
	return translated;
}

// NEW
void print (menu_t *menu, game_t * game, char * string, int * y, int * x, int size, int color_index) {
	int i = 0;
	unsigned char c = 0;
	unsigned char *translated = NULL;
	translated = translate(string);
	
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


/*
void print (menu_t *menu, game_t * game, char * string, int * y, int * x, int size, int color_index) {
	int i = 0;
	char c;
	while ( (c = string[i]) != '\0') {
		i++;
		print_char (menu, game, *y, *x, c, size, color_index);
		*x+=8*size;
	}
}*/

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
		y = 110;	// +8
		x = 130; 	// +16
	} else {
		y = 97;		// +8
		x = 50; 	// +16	
	}
	int row = y;
	int col = x;
	int height = font_rom8x16.height;	// 16
	int width = font_rom8x16.maxwidth;	//  8
	
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
		
		// textovy popisek
		char *string = menu->item_names[i+menu->language*NBR_ITEM];
		if ( row >= y) {			// text neni pod logem
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
		if ( row >= y) {			// text neni pod logem
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
					if (menu->selected_player == 0) {
						string = menu->color_names[game->player1.color_index+menu->language*NBR_COLOR];
						print (menu, game, string, &row, &col, size, game->player1.color_index);
					} else if (menu->selected_player == 1) {
						string = menu->color_names[game->player2.color_index+menu->language*NBR_COLOR];
						print (menu, game, string, &row, &col, size, game->player2.color_index);
					} else if (menu->selected_player == 2) {
						string = menu->color_names[game->player3.color_index+menu->language*NBR_COLOR];
						print (menu, game, string, &row, &col, size, game->player3.color_index);
					}
					break;
				case NUMBER_OF_BOTS:
					string =num_to_str (game->bots);
					print (menu, game, string, &row, &col, size, WHITE);
					break;
				case DIFFICULTY:
					string = num_to_str (menu->speed);
					print (menu, game, string, &row, &col, size, WHITE);
					break;
				case RESTART:
					if (menu->restart == 1) {
						print (menu, game, menu->words[TRUE+menu->language*NBR_WORD], &row, &col, size, GREEN);
					} else {
						print (menu, game, menu->words[FALSE+menu->language*NBR_WORD], &row, &col, size, RED);
					}
					break;
				case START:
					col+=2*width*size;
					if (menu->start == 1) {
						print (menu, game, menu->words[TRUE+menu->language*NBR_WORD], &row, &col, size, GREEN);
					} else {
						print (menu, game, menu->words[FALSE+menu->language*NBR_WORD], &row, &col, size, RED);
					}
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
	int shift = (game->bots+game->players)/2;	//3;	//
	int index = (game->bots+game->players) - shift;		//  - shift
	char *string = NULL;
	int height = font_rom8x16.height;	// 16
	int width = font_rom8x16.maxwidth;	//  8
	
	printf("Defoult posun: %d	index: %d/%d\n", position, index, game->bots+game->players);
	while (index != 0) {
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
		wheel_position(0, &menu->new_positon_w0);
		
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
			print (menu, game, menu->words[CONTINUE+menu->language*NBR_WORD], &y, &x, size, YELLOW);
		}
		render_gui(WIDTH, HEIGHT, game->frame_buffer);
	}	// konec while
	sleep(1);
}

//--- Terminal ----------------------------------------------------------------

void print_menu(menu_t *menu, game_t * game) {
	if (DEBUG) {
		//printf("Die Kurve : menu\n");
		for (int i = 0; i < NBR_ITEM; i++) {
			printf("	");
			
			// vybrany prvek menu
			printf("[");
			if ( i == menu->item) {
				printf("x");
			} else {
				printf(" ");
			}
			printf("]");
			
			// textovy popisek
			printf(" %s", menu->item_names[i+menu->language*NBR_ITEM]);
			// X vyber polozky za dvojteckou
			if ( i == menu->item) {
				printf(" : ");
			} else {
				printf("	");
			}
			// sub menu
			switch (i) {
				case LANGUAGE:
					printf(" %s", menu->languages[menu->language]);
					break;
				case TEXT_SIZE:
					printf(" %d", menu->size);
					break;
				case NUMBER_OF_PLAYERS:
					printf(" %d", game->players);
					break;
				case PLAYER:
					printf(" %d", menu->selected_player+1);
					break;
				case COLOR:
					if (menu->selected_player == 0) {
						printf(" %s", menu->color_names[game->player1.color_index+menu->language*NBR_COLOR]);
					} else if (menu->selected_player == 1) {
						printf(" %s", menu->color_names[game->player2.color_index+menu->language*NBR_COLOR]);
					} else if (menu->selected_player == 2) {
						printf(" %s", menu->color_names[game->player3.color_index+menu->language*NBR_COLOR]);
					}
					break;
				case NUMBER_OF_BOTS:
					printf(" %d", game->bots);
					break;
				case DIFFICULTY:
					printf(" %d", menu->speed);
					break;
				case RESTART:
					printf(" %s", menu->restart == 1 ? "true" : "false");
					break;
				case START:
					printf(" %s", menu->start == 1 ? "true" : "false");
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

/*
 [ ] 
 [x] 
 	
	//printf("\033[1J");		// clear
	//printf("\033[H");		// go home!
	
	printf("Font size: %d\n", font_rom8x16.size); 
	int x = 60, y = 70, index = 175, size = 2;
	//open_img("dieKurve.ppm", &game->img);
	int ;
	print_char (game, y, x, index, size);
	xwin_redraw(WIDTH, HEIGHT, game->img);
	
	while ( getchar() != 'q') {
		getline(&line, &n, fd);
		printf("%s", line);
	}
*/

/* end of gui.c */
