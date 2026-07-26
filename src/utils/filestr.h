#ifndef FILESTR_H
#define FILESTR_H

#include "types.h"

char **splitIntoLines(const char *const filename, udlong *length);
char **splitIntoSymbols(const char *const filename, const char *const symbols);

void trimSpaces(char *const string);
void trimAfterSymbols(char *const string, const char *const symbols);
// void trimBeforeSymbols(char *const string, const char *const symbols);

// int countSymbolsInFile(const char *const filename, const char *const symbols);
int findSymbols(const char *const symbols); // найти символ, вернуть его индекс в массиве или -1

#endif