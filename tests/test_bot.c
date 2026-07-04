#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "../src/bot.h"

void test_bot_init() {
    bot_t bot;
    bot_init(&bot);
    
    if (bot.alive != false) {
        printf("Test failed: bot.alive should be false after initialization.\n");
        assert(bot.alive == false);
    }
    if (bot.color != 0) {
        printf("Test failed: bot.color should be 0 after initialization.\n");
        assert(bot.color == 0);
    }
    if (bot.position.x != 0 || bot.position.y != 0) {
        printf("Test failed: bot.position should be (0, 0) after initialization.\n");
        assert(bot.position.x == 0 && bot.position.y == 0);
    }
    if (bot.vector.x != 0 || bot.vector.y != 0) {
        printf("Test failed: bot.vector should be (0, 0) after initialization.\n");
        assert(bot.vector.x == 0 && bot.vector.y == 0);
    }
    if (bot.direction != 0) {
        printf("Test failed: bot.direction should be 0 after initialization.\n");
        assert(bot.direction == 0);
    }
}

void test_bot_set_random_position() {
    bot_t bot;
    srand(1);

    bot_set_random_position(&bot, 20, 20);

    assert(bot.position.x >= 5);
    assert(bot.position.x <= 14);
    assert(bot.position.y >= 5);
    assert(bot.position.y <= 14);
}

void test_bot_set_random_vector() {
    bot_t bot;
    srand(1);

    bot_set_random_vector(&bot);

    assert(bot.vector.x >= -1 && bot.vector.x <= 1);
    assert(bot.vector.y >= -1 && bot.vector.y <= 1);
    assert(bot.vector.x != 0 || bot.vector.y != 0);
}

void test_bots_random_setup_invalid_arguments() {
    bot_t bots[MAX_BOTS];

    assert(bots_random_setup(NULL, 1, 20, 20) == false);
    assert(bots_random_setup(bots, 0, 20, 20) == false);
    assert(bots_random_setup(bots, MAX_BOTS + 1, 20, 20) == false);
}

void test_bots_random_setup() {
    bot_t bots[MAX_BOTS];
    srand(1);

    assert(bots_random_setup(bots, 3, 20, 20) == true);

    for (int i = 0; i < 3; i++) {
        assert(bots[i].alive == false);
        assert(bots[i].position.x >= 5);
        assert(bots[i].position.x <= 14);
        assert(bots[i].position.y >= 5);
        assert(bots[i].position.y <= 14);
        assert(bots[i].vector.x >= -1 && bots[i].vector.x <= 1);
        assert(bots[i].vector.y >= -1 && bots[i].vector.y <= 1);
        assert(bots[i].vector.x != 0 || bots[i].vector.y != 0);
        assert(bots[i].direction == 0);
        assert(bots[i].color != 0);
    }
}