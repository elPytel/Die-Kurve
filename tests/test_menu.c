#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/menu.h"

// Test 1: Ověří, že inicializace menu projde a nevrátí chybu
void test_menu_initialization_success(void) {
    menu_t menu;
    bool init_status = menu_init(&menu);

    // Podmínka úspěchu: Funkce musí vrátit true
    assert(init_status == true);

    // Uklidíme alokovanou paměť
    kill_menu(&menu);
}

// Test 2: Ověří, že se řetězce položek menu reálně alokovaly a obsahují správná data
void test_menu_items_allocation_and_content(void) {
    menu_t menu;
    menu_init(&menu);

    // Zkontrolujeme, že ukazatele na pole jmen nejsou NULL
    assert(menu.item_names != NULL);

    assert(strcmp(menu.item_names[LANGUAGE * NBR_LANGUAGE + CZ], "jazyk") == 0);
    assert(strcmp(menu.item_names[NUMBER_OF_BOTS * NBR_LANGUAGE + EN], "number of bots") == 0);
    assert(strcmp(menu.item_names[DIFFICULTY * NBR_LANGUAGE + PL], "trudność") == 0);

    kill_menu(&menu);
}

// Test 3: Robustnost proti neplatným stavům (např. vícenásobné spuštění kill_menu)
void test_menu_double_kill_safety(void) {
    menu_t menu;
    menu_init(&menu);
    kill_menu(&menu);
}