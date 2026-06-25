#include "logger.h"

// Tady se skutečně alokuje místo pro handle
FILE *g_log_file = NULL;

void logger_init(const char *filename) {
    if (g_log_file == NULL) {
        g_log_file = fopen(filename, "a");
    }
}

void logger_close(void) {
    if (g_log_file) {
        fclose(g_log_file);
        g_log_file = NULL;
    }
}