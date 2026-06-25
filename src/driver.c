// by Pytel

#include <SDL2/SDL.h>
#include "driver.h"

#define DEBUG 1

static board_t virtual_board = { .encoder_w0 = 0, .encoder_w1 = 0 };

void pool_events () {
	SDL_Event event;
    // Dočasné int proměnné, aby správně fungovalo sčítání/odčítání před modulo operací
    int uhel_w0 = virtual_board.encoder_w0;
    int uhel_w1 = virtual_board.encoder_w1;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            kill_gui();
            exit(0);
        }

        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;

            // Enkodér 0 (DF)
            if (key == SDLK_d) {
                uhel_w0 -= 30;
            } else if (key == SDLK_f) {
                uhel_w0 += 30;
            }
            
            // Enkodér 1 (JK)
            if (key == SDLK_j) {
                uhel_w1 -= 30;
            } else if (key == SDLK_k) {
                uhel_w1 += 30;
            }
        }
    }

    // Uložíme ošetřené hodnoty (rozsah 0-255) zpět do struktury desky
    virtual_board.encoder_w0 = (uhel_w0 + 256) % 256;
    virtual_board.encoder_w1 = (uhel_w1 + 256) % 256;
}

bool wheel_position(int number, uint8_t *degree) {
    uint8_t stara_hodnota = *degree;

    if (number == 0) {
        *degree = virtual_board.encoder_w0;
    } else if (number == 1) {
        *degree = virtual_board.encoder_w1;
    } else {
        return false; // Neplatné číslo enkodéru
    }

    if (DEBUG && (stara_hodnota != *degree)) {
        printf("DEBUG Hardware -> Enkodér %d změnil úhel na: %d\n", number, *degree);
    }

    return true;
}
bool RGB_LED (int number, color_t color) {
	if (number > 2) {
		return false;
	}
	return true;
}

// output 32 LEDs
void LED_stripe (int number) {}

#include <termios.h> 
void Call_termios(int reset) {
	static struct termios tio, tioOld;
	tcgetattr(STDIN_FILENO, &tio);
	if (reset) {
		tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
	} else {
		tioOld = tio; //backup 
		cfmakeraw(&tio);
		tio.c_oflag |= OPOST;
		tcsetattr(STDIN_FILENO, TCSANOW, &tio);
	}
}

//--- SDL ----------------------------------------------------------

unsigned char *img = NULL;

bool gui_init () {
	bool ret = false;
	img = (unsigned char*)malloc(3*WIDTH*HEIGHT *sizeof(unsigned char));
	if ( xwin_init(WIDTH, HEIGHT) == 0 ) {
		ret = true;
	}
	return ret;
}

bool kill_gui () {
	xwin_close();
	free(img);
	return true;
}

bool render_gui (int w, int h, uint16_t *frame_buffer) {
	uint16_t pixel = 0;
	
	for (int index = 0; index < WIDTH*HEIGHT; index++) {
		pixel = frame_buffer[index];
		img[3*index +0] = (unsigned char)(pixel >> 8) & 0b11111000;
		img[3*index +1] = (unsigned char)(pixel >> 3) & 0b11111100;
		img[3*index +2] = (unsigned char)(pixel << 3) & 0b11111000;
		if (0 && DEBUG && pixel) {
			printf("R: %d G: %d B: %d	16b pixel color: %d\n", img[3*index +0], img[3*index +1], img[3*index +2], pixel);
			printf("R: %d \n", (unsigned char)pixel);
		}
	}
	xwin_redraw(w, h, img);
	return true;
}

/* end of gui.c */
