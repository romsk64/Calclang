/** filestr.c - утилита для
 *                  - получения массива строк файла
 *                  - получения массива строк по определенному символу
 *                  - обрезки лишних пробелов
 *                  - удаления определенных частей файла после определенной последовательности символов
 */

#include "filestr.h"
#include "fexists.h"

char **splitIntoLines(const char *const filename) { // массив строк файла
    if (isFileExists(filename));
}

char **splitIntoSymbols(const char *const filename, const char *const symbols) { // последовательность символов тоже поддерживается

}

void trimSpaces(char *const string) { // меняются сами данные

}

void trimSpacesAfterSymbols(char *const string, const char *const symbols) {

}

void trimSpacesBeforeSymbols(char *const string, const char *const symbols) {

}