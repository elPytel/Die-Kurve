/**
 * @file driver.h
 * @brief Driver interface for the game, providing functions for GUI initialization, LED control, and rotary encoder handling.
 * @author Pytel
 */

#ifndef __DRIVER_h__
#define __DRIVER_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "game.h"

/**
 * @brief Sets the position of the rotary encoder.
 * @param number The encoder number (0-2).
 * @param degree Pointer to the variable to store the rotation angle.
 * @return true if successful, false otherwise.
 */
bool wheel_position(int number, uint8_t *degree);	// , bool *press

/**
 * @brief Turns on the RGB LED at the specified position.
 * @param number The LED number (0-31).
 * @param color The color to display on the LED.
 * @return true if successful, false otherwise.
 */
bool RGB_LED (int number, color_t color);

/**
 * Not implemented in this version.
 * 
 * @brief Controls a stripe of 32 LEDs.
 * @param number The LED number (0-31).
 */
void LED_stripe (int number);

void Call_termios(int reset);

#include "xwin_sdl.h"

# define HEIGHT 320
# define WIDTH 	480

	//240	//480
	//320	//640

/**
 * @brief Initializes the GUI for the game.
 * @return true if the GUI was initialized successfully, false otherwise.
 */
bool gui_init ();

/**
 * @brief Closes the GUI and frees allocated resources.
 * @return true if the GUI was closed successfully, false otherwise.
 */
bool kill_gui ();

/**
 * @brief Converts a 16-bit color matrix to an RGB image.
 * @param w width of the image
 * @param h height of the image
 * @param frame_buffer pointer to the frame buffer containing 16b color data
 * @return true if the conversion and rendering were successful, false otherwise
 */
bool render_gui (int w, int h, uint16_t *img);

#endif
/* end of driver.h */