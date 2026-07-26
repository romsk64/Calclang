/** Виртуальная машина Calclang
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils/fexists.h"

int main(int argc, char *argv[]) {
    char *filename;

    if (argc > 2) {
        printf("Too many arguments!");
        return 1;
    } else if (argc == 2) {

        filename = malloc(sizeof(argv[1]));
        *filename = argv[1];
    }
    
    return 0;
}