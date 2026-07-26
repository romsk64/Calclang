#include <stdio.h>
#include <stdlib.h>
#include "fexists.h"

int isFileExists(const char *const string) { // 0 - не существет, 1 - существует
    FILE *file = fopen(string, "r");

    if (file == NULL) {
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}