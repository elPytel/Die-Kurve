#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/menu.h"

void test_menu_initialization_success(void) {
    menu_t menu;
    bool init_status = menu_init(&menu);
    assert(init_status == true);
    kill_menu(&menu);
}

void test_menu_items_allocation_and_content(void) {
    menu_t menu;
    menu_init(&menu);

    assert(menu.item_names != NULL);

    assert(strcmp(menu.item_names[LANGUAGE * NBR_LANGUAGE + CZ], "jazyk") == 0);
    assert(strcmp(menu.item_names[NUMBER_OF_BOTS * NBR_LANGUAGE + EN], "number of bots") == 0);
    assert(strcmp(menu.item_names[DIFFICULTY * NBR_LANGUAGE + PL], "trudność") == 0);

    kill_menu(&menu);
}
