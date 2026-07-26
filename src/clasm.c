/** Ассемблер CLVM
 * 
 */

#include <stdio.h>
#include "utils/fexists.h"

typedef enum bytecodecmd {
    NOTHING, // 0x00
    d
} bytecodecmd;

int ascomp(const char *filename, const char *outfilename) {
    FILE *file = fopen(filename, "r");
    // FILE *bytecode = fopen(outfilename, "wb");

    fclose(file);
    // fclose(bytecode);

    bytecodecmd my = NOTHING;
}

int main(int argc, char *argv[]) {
    if (argc == 3) {
        if (isFileExists(argv[1])) {

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