#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

// Deklarace externího přístupu k souborovému handle
extern FILE *g_log_file;

// Makro, které zpříjemní volání
#define logger_log(...) do { \
    if (g_log_file) { \
        fprintf(g_log_file, "[LOG] " __VA_ARGS__); \
        fprintf(g_log_file, "\n"); \
        fflush(g_log_file); \
    } \
} while(0)

void logger_init(const char *filename);
void logger_close(void);

#endif