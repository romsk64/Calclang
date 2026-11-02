#include <string.h>
#include <stdint.h>

uint64_t cnvStrdecToInt64(const char *restrict string) {
    if (string == NULL) return 0;

    register size_t stringLength = strlen(string);

    uint8_t allDigits[4096];
    register uint16_t digitsPlacesCount = 0;

    for (size_t i = 0; i < stringLength; i++) {
        if (string[i] == '0') {
            allDigits[i] = 0;
            digitsPlacesCount++;
        }
        else if (string[i] == '1') {
            allDigits[i] = 1;
            digitsPlacesCount++;
        }
        else if (string[i] == '2') {
            allDigits[i] = 2;
            digitsPlacesCount++;
        }
        else if (string[i] == '3') {
            allDigits[i] = 3;
            digitsPlacesCount++;
        }
        else if (string[i] == '4') {
            allDigits[i] = 4;
            digitsPlacesCount++;
        }
        else if (string[i] == '5') {
            allDigits[i] = 5;
            digitsPlacesCount++;
        }
        else if (string[i] == '6') {
            allDigits[i] = 6;
            digitsPlacesCount++;
        }
        else if (string[i] == '7') {
            allDigits[i] = 7;
            digitsPlacesCount++;
        }
        else if (string[i] == '8') {
            allDigits[i] = 8;
            digitsPlacesCount++;
        }
        else if (string[i] == '9') {
            allDigits[i] = 9;
            digitsPlacesCount++;
        }

        else {
            return 0;
        }
    }

    for (uint16_t i = 0; i < digitsPlacesCount; i++)
    return result;
}
