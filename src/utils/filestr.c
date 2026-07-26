/** filestr.c - утилита для
 *                  - получения массива строк файла
 *                  - получения массива строк по определенному символу
 *                  - обрезки лишних пробелов
 *                  - удаления определенных частей файла после определенной последовательности символов
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filestr.h"
#include "fexists.h"
#include "types.h"
#define MAX_BUFFER_SIZE 2048

char **splitIntoLines(const char *const filename, udlong *length) { // массив строк файла
    char **resultArray = NULL;
    char *buffer = malloc(MAX_BUFFER_SIZE);
    udlong lineCount = 0;

    if (isFileExists(filename)) {
        FILE *file = fopen(filename, "r");

        for (lineCount; fgets(buffer, MAX_BUFFER_SIZE, file) != NULL; lineCount++); // подсчет количества строк
        resultArray = calloc(lineCount, sizeof(char*));
        rewind(file);

        for (udlong line = 0; fgets(buffer, MAX_BUFFER_SIZE, file) != NULL; line++) {
            size_t lineLength = strlen(buffer);
            resultArray[line] = malloc(lineLength + 1);

            strcpy(resultArray[line], buffer);
        }

        fclose(file);
    }

    free(buffer);
    return resultArray;
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