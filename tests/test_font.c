#include <stdio.h>
#include <stdint.h>
#include "../src/font_types.h"

void test_font_table() {
    // print all the characters in the font table
    /*
    for (int i = 0; i < 256; i++) {
        printf("Character %d:\n", i);
        for (int j = 0; j < font_rom8x16.height; j++) {
            uint16_t row = font_rom8x16.bits[i * font_rom8x16.height + j];
            for (int k = 0; k < font_rom8x16.maxwidth; k++) {
                if (row & (1 << (15 - k))) {
                    printf("#");
                } else {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }
    */
}