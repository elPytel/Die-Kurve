#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/gui.h"

void test_translate_ascii(void) {
    unsigned char *res = translate("Tron 123");
    assert(res != NULL);
    assert(strcmp((char*)res, "Tron 123") == 0);
    free(res);
}

void test_translate_empty(void) {
    assert(translate("") == NULL);
}

void test_translate_diacritics(void) {
    unsigned char *res = translate("žába");
    assert(res != NULL);
    // Podle kódování UTF-8: ž = 0x98, á = 0x95, b = 0x62, a = 0x61
    assert(res[0] == 0x98);
    assert(res[1] == 0x95);
    assert(res[2] == 0x62);
    assert(res[3] == 0x61);
    assert(res[4] == '\0'); // Ověříme, že string je správně ukončený
    free(res);
}