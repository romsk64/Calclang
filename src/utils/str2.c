/* str2.c - короче функции работы со строками + улучшенный filestr */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// checking if string starts with substring (prefix)
bool str2_startsWith(
    const char *string,
    const char *prefix
) {
    if (string == NULL || prefix == NULL) {
        return false;
    }

    while (*prefix) { // пока префикс != '\0'
        if (*string != *prefix) {
            return false;
        }

        string++;
        prefix++;
    }

    return true; // если префикс закончился '\0' и ни одного несовпадения
}

// checking if string ends with substring (postfix)
bool str2_endsWith(
    const char *string,
    const char *postfix
) {
    if (string == NULL || postfix == NULL) {
        return false;
    }

    bool result;
    const size_t postfixLength = strlen(postfix);
    const size_t stringLength = strlen(string);
    const size_t lengthDiff = stringLength - postfixLength; // разница между stringLength и postfixLength

    if (postfixLength > stringLength) {
        return false;
    }

    result = strncmp(string + lengthDiff, postfix, postfixLength) == 0;
    return result;
}
