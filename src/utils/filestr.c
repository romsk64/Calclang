/** filestr.c - утилита для
 *                  - получения массива строк файла
 *                  - получения массива строк по определенному символу
 *                  - обрезки лишних пробелов
 *                  - удаления определенных частей файла после определенной последовательности символов
 */

#include <stdio.h>
#include <stdlib.h>
#include "filestr.h"
#include "fexists.h"
#include "types.h"

char **splitIntoLines(const char *const filename, udlong *length) { // массив строк файла
    char **resultArray = calloc(1, sizeof(char*));
    int ch;
    char *feggg;

    if (isFileExists(filename)) {
        FILE *file = fopen(filename, "r");

        while ((ch = fgetc(file)) != EOF) {
            for (udlong i = 0; (char)ch != '\n'; i++) {
                //...
            }
        }

        fclose(file);
    }
}

char **splitIntoSymbols(const char *const filename, const char *const symbols) { // последовательность символов тоже поддерживается

}

void trimSpaces(char *const string) { // меняются сами данные

}

void trimAfterSymbols(char *const string, const char *const symbols) {

}

// void trimBeforeSymbols(char *const string, const char *const symbols) {

// }

int findSymbols(const char *const symbols) { // найти символ, вернуть его индекс в массиве или -1

}