/**
 * File name: aposem-main.c
 * Date:      2020/06/02
 * Author:    Jaroslav Körner, Kateřina Poláková
 * Hra pro APO desku - Achtung, die Kurve!
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "driver.h"
#include "game.h"
#include "gui.h"
#include "menu.h"

#define DEBUG 1

#define OK 1
#define NOK -1

#define TARGET_FRAME_TIME 16 /**< Target frame time in milliseconds ~ 60fps */

// - main ---------------------------------------------------------------------

int main() {
    // inicializace promenych
    uint32_t frame_start;
    int frame_time;
    char *logo_file = "./assets/dieKurve.ppm";

    gui_init();  // alokoje a smaze graficky buffer
    game_t game;
    game_init(&game);

    menu_t menu;
    menu_init(&menu);

    if (DEBUG) {  // 0 &&
        game.bots = 0;
        menu.size = 1;
        menu.speed = 10;
    }

    // nacteni herniho loga
    unsigned char *img = NULL;
    open_img(logo_file, &img);
    save_logo(img, &game);
    free(img);

    // Game loop
    while (true != false) {
        menu.start = false;
        menu.item = 0;
        while (!menu.start) {
            frame_start = SDL_GetTicks();

            pool_events();
            menu_function(&menu, &game);

            // --- SYSTÉM REGULACE FPS (ZÁMEK NA 60 FPS) ---
            // Spočítáme, jak dlouho trval samotný výpočet a vykreslení snímku
            frame_time = SDL_GetTicks() - frame_start;
            // Pokud byl výpočet rychlejší než 16.66 ms, zbytek času prospíme
            if (frame_time < TARGET_FRAME_TIME) {
                SDL_Delay(TARGET_FRAME_TIME - frame_time);
            }
            
            printf("[FPS Lock] Vypočteno za: %d ms | Celkový čas snímku: %d ms\n", frame_time, SDL_GetTicks() - frame_start);
        }
        // nastaveni hernich parametru podle dat z menu
        set_game(&menu, &game);
        while (playing(&game)) {
            printf("Game is running...\n");
            frame_start = SDL_GetTicks();
            pool_events();

            if (DEBUG) {
                printf("New turn.\n");
            }

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

            // --- SYSTÉM REGULACE FPS (ZÁMEK NA 60 FPS) ---
            // Spočítáme, jak dlouho trval samotný výpočet a vykreslení snímku
            frame_time = SDL_GetTicks() - frame_start;
            // Pokud byl výpočet rychlejší než 16.66 ms, zbytek času prospíme
            if (frame_time < TARGET_FRAME_TIME) {
                SDL_Delay(TARGET_FRAME_TIME - frame_time);
            }
            
            printf("[FPS Lock] Vypočteno za: %d ms | Celkový čas snímku: %d ms\n", frame_time, SDL_GetTicks() - frame_start);
        }
        
        frame_start = SDL_GetTicks();
        while (true) {
            pool_events();
            score_bord(&menu, &game);


            render_score_bord(&menu, &game);
            render_gui(WIDTH, HEIGHT, game.frame_buffer);

            if (DEBUG) {
                menu.restart = false;
            }
            SDL_Delay(20);
            if (frame_start + 5000 < SDL_GetTicks()) {
                break;
            }
        }
    }
    kill_gui();
    kill_menu(&menu);
    kill_game_all(&game);
    return OK;
}

/* end of aposem-main.c */
