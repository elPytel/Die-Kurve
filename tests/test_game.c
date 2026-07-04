/*
 * File name: test_game.c
 * Date:      2026/06/25
 * Author:    Pytel
 * Popis:     Unit testy pro herní logiku (game.c)
 */

#include <stdio.h>
#include <assert.h>
#include "../src/game.h"


void test_game_init() {    
    game_t game;
    bool success = game_init(&game);
    
    assert(success == true);
    assert(game.playing == false);
    assert(game.score == NULL);
    
    // Uklidíme po sobě
    game_kill_all(&game);
}

void test_player_init() {    
    game_t game;
    game_init(&game);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        assert(game.players[i].enable == false);
        assert(game.players[i].alive == false);
        assert(game.players[i].encoder_id == i);
        assert(game.players[i].color_index == i);
    }
    
    game_kill_all(&game);
}

void test_rotate_right() { // o 45 stupnu v ctvercove siti
    int y = 1, x = 0; // směrem J
    right(&y, &x);
    // JZ
    assert(y == 1 && x == -1);
    right(&y, &x);
    // Z
    assert(y == 0 && x == -1);
    right(&y, &x);
    // SZ
    assert(y == -1 && x == -1);
    right(&y, &x);
    // S
    assert(y == -1 && x == 0);
    right(&y, &x);
    // SV
    assert(y == -1 && x == 1);
    right(&y, &x);
    // V
    assert(y == 0 && x == 1);
    right(&y, &x);
    // JV
    assert(y == 1 && x == 1);
    right(&y, &x);
    // J
    assert(y == 1 && x == 0);
    
}

void test_rotate_left() {
    int y = 1, x = 0; // směrem dolů
    left(&y, &x);
    // JV
    assert(y == 1 && x == 1);
    left(&y, &x);
    // V
    assert(y == 0 && x == 1);
    left(&y, &x);
    // SV
    assert(y == -1 && x == 1);
    left(&y, &x);
    // S
    assert(y == -1 && x == 0);
    left(&y, &x);
    // SZ
    assert(y == -1 && x == -1);
    left(&y, &x);
    // Z
    assert(y == 0 && x == -1);
    left(&y, &x);
    // JZ
    assert(y == 1 && x == -1);
    left(&y, &x);
    // J
    assert(y == 1 && x == 0);
}