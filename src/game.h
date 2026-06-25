// by Pytel

#ifndef __GAME_h__
#define __GAME_h__

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h> 

#define MAX_PLAYERS 3
#define MAX_BOTS 10

/**
 * @brief color structure
 * 
 * Structure representing a color in RGB format.
 */
typedef struct {
	uint8_t R; /**< red component */
	uint8_t G; /**< green component */
	uint8_t B; /**< blue component */
} color_t;

/**
 * @brief position structure
 * 
 * Structure representing a position in 2D space.
 */
typedef struct {
	int y; /**< y-coordinate */
	int x; /**< x-coordinate */
} position_t;

/**
 * @brief player structure
 * 
 * Structure representing a player in the game.
 */
typedef struct {
	bool enable;	/**< ucastni se hry */
	bool alive;		/**< still not dead */
	int encoder_id;	/**< id enkoderu */
	// color
	int color_index;
	uint16_t color;
	// pozice
	position_t position;
	// vektor
	position_t vector; /**< movement in x and y directions */
	position_t old_vector;
} player_t;

/**
 * @brief game structure
 * 
 * Structure representing the game state.
 */
typedef struct {
	bool playing;
	// hraci
	int active_players_count;		// pocet hracu
	player_t players[MAX_PLAYERS];
	// boti
	int bots;			// počet bot ve hře
	position_t *positions;
	position_t *vectors;
	uint16_t *colors;	// pole barev botu
	bool *live_bots;
	int *directions;
	int *score;			// [active_players_count+bots] pole, ktere drzi skore pro kazdeho hrace i bota
	int *spaces;		// cerchovany mod
	int speed;			// turn time
	uint16_t *logo;			// zde je nahrane herni logo
	uint16_t *game_bord;	// veskera herni plocha - aka frame buffer
	uint16_t *frame_buffer;	// frame buffer pro vice vrstva meny
} game_t;


void player_to_string(player_t *p, char *buffer, size_t buf_size);

/**
 * @brief Returns true if the game is still running.
 * @param game Pointer to the game structure.
 * @return true if the game is still running, false otherwise.
 */
bool game_playing (game_t * game);

/**
 * @brief Initializes the game structure with default values.
 * @param game Pointer to the game structure to be initialized.
 * @return true if initialization was successful, false otherwise.
 */
bool game_init (game_t * game);

bool game_is_someon_alive (game_t * game);

/**
 * @brief Checks if any player or bot is still alive and updates the game state accordingly.
 * @param game Pointer to the game structure.
 */
void game_set_if_someon_alive (game_t * game);

// aktualizuje vektro bota
void game_AI_move (game_t * game);

// aktualizuje vektor hrace
void player_move (game_t * game);

/**
 * @brief Checks the validity of a move.
 * Evaluate points for dead active_players_count and bots and updates the game state for alive active_players_count.
 * @param game Pointer to the game structure.
 */
void game_validate_play(game_t * game);

/**
 * @brief Will calculate the score based on the number of dead players and bots.
 */
int game_calculate_score(game_t * game);

// dekonstruktor
void game_free (game_t * game);

void game_kill_all (game_t * game);

#endif
/* end of game.h */