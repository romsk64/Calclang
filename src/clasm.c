/** Ассемблер CLVM
 * 
 */

#include <stdio.h>
#include "utils/fexists.h"
#include "utils/filestr.h"
#include "utils/types.h"

enum bytecodecmd {
    PRINT = 0x01,
    PRINTMEM = 0x02,
    INPUT = 0x03,
    DUP = 0x04,
    ADD = 0x05,
    SUB = 0x06,
    MUL = 0x07, // умножение
    DIV = 0x08  // деление
};

// char **clearCode(const char *filename) {

// }

int ascomp(const char *const filename, const char *const outfilename) {
    udlong arrlength;
    char **arr = splitIntoLines(filename, &arrlength);

    for (udlong line = 0; line < arrlength; line++) {
        trimSpaces(line);
        trimAfterSymbols(line, ";");
    }
}

int main(int argc, char **argv) {
    char *filename, *outputfilename;

    if (argc == 3) {
        if (isFileExists(argv[1])) {
            filename = argv[1];
            outputfilename = argv[2];
        } else {
            perror("File not exists!\n");
            return 1;
        }
    } else if (argc < 2) {
        perror("Too little arguments!\n");
        return 1;
    } else if (argc > 3) {
        perror("Too many arguments!\n");
        return 1;
    }

    return 0;
}