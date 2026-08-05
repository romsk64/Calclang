/** filestr.c - утилита для
 *                  - получения массива строк файла
 *                  - получения массива строк по определенному символу
 *                  - обрезки лишних пробелов
 *                  - удаления определенных частей файла после определенной последовательности символов
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "filestr.h"
#include "fexists.h"
#define MAX_BUFFER_SIZE 4096
#define STD_LINE_ARRAY_SIZE 1024

char **splitIntoLines(const char *const filename, size_t *const length) { // массив строк файла, если ошибка - вернет NULL
    int stdLineArraySize = STD_LINE_ARRAY_SIZE;
    char **resultArray = NULL;

    // char *buffer = malloc(MAX_BUFFER_SIZE);

    *length = 0;

    if (isFileExists(filename)) {
        FILE *file = fopen(filename, "r");

        char *buffer = malloc(MAX_BUFFER_SIZE);
        resultArray = malloc(stdLineArraySize * sizeof(char*));

        if (buffer == NULL || resultArray == NULL) {
            printf("Error of memory allocation (NULL returned)!");
            fclose(file);
            return NULL;
        }

        // for (*length; fgets(buffer, MAX_BUFFER_SIZE, file) != NULL; (*length)++); // подсчет количества строк
        // resultArray = calloc(*length, sizeof(char*));
        // rewind(file);

        size_t line = 0;
        for (line; /*line < *length &&*/ fgets(buffer, MAX_BUFFER_SIZE, file) != NULL; line++) {
            if (line > stdLineArraySize) {
                stdLineArraySize *= 2;
                char **tempArray = realloc(resultArray, stdLineArraySize * sizeof(char*));

                if (tempArray == NULL) {
                    printf("Error of mwmory allocation (returned NULL)!");

                    freeSplitLines(resultArray, line);
                    fclose(file);
                    return NULL;
                } else {
                    resultArray = tempArray;
                }
            }

            size_t lineLength = strlen(buffer);
            resultArray[line] = malloc(lineLength + 1);

            strcpy(resultArray[line], buffer);
        }
        *length = line;

        fclose(file);
        free(buffer);
    }

    // free(buffer);
    return resultArray;
}

char **splitIntoSymbols(const char *const filename, const char *const symbols) { // последовательность символов тоже поддерживается

}

void freeSplitLines(const char **const splitLines, const size_t *const length) {
    for (size_t i = 0; i < *length; i++) {
        free(splitLines[i]);
    }

    free(splitLines);
}

int trimSpaces(char *const string) { // меняются сами данные
    if (string == NULL) return 0;

    size_t firstSymbol;
    size_t lastSymbol; // первый и последний символ, не являющийся пробелом или табом

    size_t stringSize = strlen(string);
    if (stringSize == 0) return 0;

    for (firstSymbol = 0; firstSymbol < stringSize; firstSymbol++) {
        if ((string[firstSymbol] != ' ' && string[firstSymbol] != '\t') && string[firstSymbol] != '\n' && string[firstSymbol] != '\r') {
            break;
        }
    }

    if (firstSymbol == stringSize) { // только из пробелов
        string[0] = '\0';
        return 0;
    }

    for (lastSymbol = (stringSize - 1); lastSymbol >= firstSymbol; lastSymbol--) {
        if (string[lastSymbol] != ' ' && string[lastSymbol] != '\t' && string[lastSymbol] != '\n' && string[lastSymbol] != '\r') {
            break;
        }
    }

    size_t newStringSize = (lastSymbol - firstSymbol) + 1;

    if (firstSymbol != 0) {
        memmove(string, string + firstSymbol, newStringSize); // перемещение в начало, если в начале есть пробелы
    } string[newStringSize] = '\0';

    return 0;
}

int trimAfterSymbols(char *const string, const char *const symbols) { // -1 если не найдено
    bool isFound = false;

    size_t symbolsIndex = findSymbols(string, symbols, &isFound);

    return 0;
}

// void trimBeforeSymbols(char *const string, const char *const symbols) {

// }

size_t findSymbols(const char *const string, const char *const symbols, bool *const isFound) {
                                                                            // найти символ, вернуть его индекс в массиве или -1
    if (isFound == NULL) return 0;
    if (*isFound) {
        *isFound = false;
    }

    if (string == NULL || symbols == NULL) {
        return 0;
    }
    char *symbolsIndexPtr = strstr(string, symbols);
    if (symbolsIndexPtr == NULL) {
        return 0;
    }

    if (symbolsIndexPtr == NULL) {
        return 0;
    } else {
        *isFound = true;
        return (size_t)(symbolsIndexPtr - string);
    }
}