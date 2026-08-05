#ifndef FILESTR_H
#define FILESTR_H

#include <stdbool.h>
#include <stddef.h>

char **splitIntoLines(const char *const filename, size_t *const length);
char **splitIntoSymbols(const char *const filename, const char *const symbols);
void freeSplitLines(const char **const splitLines, const size_t *const length);

int trimSpaces(char *const string);
int trimAfterSymbols(char *const string, const char *const symbols);
// void trimBeforeSymbols(char *const string, const char *const symbols);

size_t findSymbols(const char *const string, const char *const symbols, bool *const isFound); // найти символ, вернуть его индекс

#endif