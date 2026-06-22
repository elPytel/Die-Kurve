// by Pytel

#include "driver.h"

#define DEBUG 1

bool wheel_position(int number, uint8_t *degree) {
	Call_termios(0);
	int uhel = *degree+256;
	char c = getchar();
	Call_termios(1);
	if (number == 0) {
		if ( c == 'd' ) {
			uhel -= 30;
		} else if ( c == 'f' ) {
			uhel += 30;
		}
	} else if (number == 1) {
		if ( c == 'j' ) {
			uhel -= 30;
		} else if ( c == 'k' ) {
			uhel += 30;
		}
	}
	*degree = uhel%256;
	if (DEBUG) {
		printf("Uhel: %d\n", *degree);
	}
	/*
	int uhel = -1;
	printf("Enter new angle: ");
	if (scanf ("%d", &uhel) != 1) {
		printf("ERROR: wrong input!\r\n");
		return false;
	} else if ( uhel <= 360 && uhel >= 0) {
		*degree = uhel;
		return true;
	} else {
		printf("ERROR: invlaid angle!\r\n");
	}*/
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

// prevede matici 16b color fromatu na RGB img
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
