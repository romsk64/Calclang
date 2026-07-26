#ifndef FILESTR_H
#define FILESTR_H

char **splitIntoLines(const char *const filename);
char **splitIntoSymbols(const char *const filename, const char *const symbols);

void trimSpaces(char *const string);
void trimSpacesAfterSymbols(char *const string, const char *const symbols);
void trimSpacesBeforeSymbols(char *const string, const char *const symbols);

#endif