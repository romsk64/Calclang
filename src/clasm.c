/** Ассемблер CLVM
 * 
 */

#include <stdio.h>
#include <stddef.h>
#include "utils/fexists.h"
#include "utils/filestr.h"
#include "utils/swapvalue.h"
#include "bytecode.h"

int ascomp(const char *const filename, const char *const outputfilename) {
    size_t arrlength;
    char **arr = splitIntoLines(filename, &arrlength);

    for (size_t line = 0; line < arrlength; line++) {
        trimAfterSymbols(line, ";");
        trimSpaces(line);
    }
}

int main(int argc, char **argv) {
    char *filename, *outputfilename;

    if (argc == 3) {
        if (isFileExists(argv[1])) {
            filename = argv[1];
            outputfilename = argv[2];
        } else {
            printf("File not exists!\n");
            return 1;
        }
    } else if (argc == 2) {
        if (isFileExists(argv[1])) {
            filename = argv[1];
            outputfilename = "a.bin";
        } else {
            printf("File not exists!\n");
            return 1;
        }
    } else if (argc < 2) {
        printf("Too little arguments!\n");
        return 1;
    } else if (argc > 3) {
        printf("Too many arguments!\n");
        return 1;
    }

    return 0;
}
