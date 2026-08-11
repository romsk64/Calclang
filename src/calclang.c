/** Виртуальная машина Calclang
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils/fexists.h"
#include "utils/swapvalue.h"
#include "bytecode.h"

#define ERRCODE_2 "[calclang]: Incorrect signature of executable file! (code 2)"
#define ERRCODE_3 "[calclang]: Error of reading file! (code 3)"
#define ERRCODE_4 "[calclang]: Error of memory allocation! (code 4)"
#define ERRCODE_6 "[calclang]: Address error! (code 6)"
#define ERRCODE_7 "[calclang]: Opcode don't supporting register! (code 7)"
#define ERRCODE_8 "[calclang]: Metadata error! (code 8)"

// METADATA_LENGTH = 4 + (sizeof(uint32_t) * 2) + sizeof(uint8_t)
#define METADATA_LENGTH 13
#define INT32_SIZE sizeof(int32_t)

int ariphmeticAction(int64_t (*function)(int64_t, int64_t), uint8_t firstRegisterInt, uint8_t secondRegisterInt, int32_t firstRegisterIndex, int32_t secondRegisterIndex, FILE *file, int8_t *ax, int16_t *bx, int32_t *cx, int64_t *dx) {
    if (firstRegisterInt == 5 || secondRegisterInt == 5) {
        fclose(file);
        printf(ERRCODE_7);
        return 7;
    } else if (firstRegisterInt == secondRegisterInt) {
        switch (firstRegisterInt) {
            case 1:
                ax[firstRegisterIndex] = (int8_t)(function(ax[firstRegisterIndex], ax[secondRegisterIndex]));
                break;
            case 2:
                bx[firstRegisterIndex] = (int16_t)(function(bx[firstRegisterIndex], bx[secondRegisterIndex]));
                break;
            case 3:
                cx[firstRegisterIndex] = (int32_t)(function(cx[firstRegisterIndex], cx[secondRegisterIndex]));
                break;
            case 4:
                dx[firstRegisterIndex] = (int64_t)(function(dx[firstRegisterIndex], dx[secondRegisterIndex]));
                break;

            default:
                fclose(file);
                printf("[calclang]: Unknown register 0x%02X! (code 5)", firstRegisterInt);
                return 5;
        }
    } else {
        switch (firstRegisterInt) {
            case 1:
                switch (secondRegisterInt) {
                    case 2:
                        ax[firstRegisterIndex] = (int8_t)(function(ax[firstRegisterIndex], bx[secondRegisterIndex]));
                        break;
                    case 3:
                        ax[firstRegisterIndex] = (int8_t)(function(ax[firstRegisterIndex], cx[secondRegisterIndex]));
                        break;
                    case 4:
                        ax[firstRegisterIndex] = (int8_t)(function(ax[firstRegisterIndex], dx[secondRegisterIndex]));
                        break;

                    default:
                        fclose(file);
                        printf("[calclang]: Unknown register 0x%02X! (code 5)", secondRegisterInt);
                        return 5;
                }
                break;
            case 2:
                switch (secondRegisterInt) {
                    case 1:
                        bx[firstRegisterIndex] = (int16_t)(function(bx[firstRegisterIndex], ax[secondRegisterIndex]));
                        break;
                    case 3:
                        bx[firstRegisterIndex] = (int16_t)(function(bx[firstRegisterIndex], cx[secondRegisterIndex]));
                        break;
                    case 4:
                        bx[firstRegisterIndex] = (int16_t)(function(bx[firstRegisterIndex], dx[secondRegisterIndex]));
                        break;

                    default:
                        fclose(file);
                        printf("[calclang]: Unknown register 0x%02X! (code 5)", secondRegisterInt);
                        return 5;
                }
                break;
            case 3:
                switch (secondRegisterInt) {
                    case 1:
                        cx[firstRegisterIndex] = (int32_t)(function(cx[firstRegisterIndex], ax[secondRegisterIndex]));
                        break;
                    case 2:
                        cx[firstRegisterIndex] = (int32_t)(function(cx[firstRegisterIndex], bx[secondRegisterIndex]));
                        break;
                    case 4:
                        cx[firstRegisterIndex] = (int32_t)(function(cx[firstRegisterIndex], dx[secondRegisterIndex]));
                        break;

                    default:
                        fclose(file);
                        printf("[calclang]: Unknown register 0x%02X! (code 5)", secondRegisterInt);
                        return 5;
                }
                break;
            case 4:
                switch (secondRegisterInt) {
                    case 1:
                        dx[firstRegisterIndex] = (int64_t)(function(dx[firstRegisterIndex], ax[secondRegisterIndex]));
                        break;
                    case 2:
                        dx[firstRegisterIndex] = (int64_t)(function(dx[firstRegisterIndex], bx[secondRegisterIndex]));
                        break;
                    case 3:
                        dx[firstRegisterIndex] = (int64_t)(function(dx[firstRegisterIndex], cx[secondRegisterIndex]));
                        break;

                    default:
                        fclose(file);
                        printf("[calclang]: Unknown register 0x%02X! (code 5)", secondRegisterInt);
                        return 5;
                }
                break;

            default:
                fclose(file);
                printf("[calclang]: Unknown register 0x%02X! (code 5)", firstRegisterInt);
                return 5;
        }
    }

    return 0;
}

int64_t add_op(int64_t x, int64_t y) {
    return (x + y);
} int64_t sub_op(int64_t x, int64_t y) {
    return (x - y);
} int64_t mul_op(int64_t x, int64_t y) {
    return (x * y);
} int64_t div_op(int64_t x, int64_t y) {
    if (y == 0) return 0;
    return (x / y);
}

size_t getFileLength(FILE *restrict file, bool *restrict isError) {
    long initialPos = ftell(file);
    if (initialPos == -1L) {
        printf(ERRCODE_3);
        *isError = true;
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size_t result = ftell(file);

    fseek(file, initialPos, SEEK_SET);
    return result;
}

bool readRegisterSize(FILE *restrict file, uint32_t *restrict registerSize) { // true если ошибка
    if (fread(registerSize, sizeof(uint32_t), 1, file) != 1) {
        fclose(file);
        printf(ERRCODE_3);
        return true;
    }

    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        *registerSize = swapUint32(*registerSize);
    #endif

    return false;
}
bool checkAllocError(FILE *restrict file, void *restrict ptr) { // true если ошибка
    if (ptr == NULL) {
        fclose(file);
        printf(ERRCODE_4);
        return true;
    }

    return false;
}

int executeBytecode(const char *const filename) {
    if (!isFileExists(filename)) {
        printf(ERRCODE_3);
        return 3;
    }

    uint32_t registerSize;
    uint8_t opcode;
    FILE *file = fopen(filename, "rb");

    size_t fileLength;
    {
        bool isError;
        fileLength = getFileLength(file, &isError);

        if (isError) {
            fclose(file);
            printf(ERRCODE_3);
            return 3;
        }
    }
    

    // заранее определение указателей на массивы данных
    int8_t *ax = NULL; // ax - 8 бит
    int16_t *bx = NULL; // bx - 16 бит
    int32_t *cx = NULL; // cx - 32 бита
    int64_t *dx = NULL; // dx - 64 бита
    char **stx = NULL; // stx - строки

    // проверка на длину метаданных
    if (fileLength < METADATA_LENGTH) {
        fclose(file);
        printf(ERRCODE_8);
        return 8;
    }

    // проверка сигнатуры
    {
        char signature[4];
        fread(signature, sizeof(char), 4, file);

        if (!(signature[0] == 'C' && signature[1] == 'L' && signature[2] == 'V' && signature[3] == 'M')) {
            fclose(file);
            printf(ERRCODE_2);
            return 2;
        }
    }


    // выделение памяти
    if (readRegisterSize(file, &registerSize)) return 3;
    ax = malloc(sizeof(int8_t) * registerSize);
    if (checkAllocError(file, ax)) return 4;

    if (readRegisterSize(file, &registerSize)) return 3;
    bx = malloc(sizeof(int16_t) * registerSize);
    if (checkAllocError(file, bx)) return 4;

    if (readRegisterSize(file, &registerSize)) return 3;
    cx = malloc(sizeof(int32_t) * registerSize);
    if (checkAllocError(file, cx)) return 4;

    if (readRegisterSize(file, &registerSize)) return 3;
    dx = malloc(sizeof(int64_t) * registerSize);
    if (checkAllocError(file, dx)) return 4;

    if (readRegisterSize(file, &registerSize)) return 3;
    stx = calloc(sizeof(char*), registerSize);
    if (checkAllocError(file, stx)) return 4;

    // определение смещений
    uint8_t addrRDATA; // адрес rdata
    // uint16_t addrSRDATA; // адрес srdata
    uint32_t addrTEXT; // адрес text
    bool addrRDATA_initialized = false;

    fread(&addrRDATA, sizeof(uint8_t), 1, file);
    // fread(&addrSRDATA, sizeof(uint16_t), 1, file);
    fread(&addrTEXT, sizeof(uint32_t), 1, file);
    
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        addrTEXT = swapUint32(addrTEXT);
    #endif

    if (addrTEXT == 0) {
        printf(ERRCODE_6);
        return 6;
    }

    size_t ip = 0; // instruction pointer
    ip = addrTEXT;
    fseek(file, addrTEXT, SEEK_SET);

    while (fread(&opcode, 1, 1, file) == 1) {
        ip++;
        printf("IP: 0x%02X\n", ip);
        printf("OPC: 0x%02X\n", opcode);

        if (opcode == FILEEND) break;
        else {
            switch (opcode) {
                case (enum bytecodecmd)PRINT:
                    {
                        uint32_t readingAddress;
                        fread(&readingAddress, sizeof(uint32_t), 1, file);
                        ip += sizeof(uint32_t);
                        
                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                            readingAddress = swapUint32(readingAddress);
                        #endif

                        fseek(file, readingAddress, SEEK_SET);

                        if (readingAddress < addrTEXT) {
                            uint8_t numberRegister;
                            fread(&numberRegister, sizeof(uint8_t), 1, file);

                            switch (numberRegister) {
                                case 0x01: // 1 байт
                                    int8_t value8;
                                    fread(&value8, sizeof(int8_t), 1, file);

                                    printf("%d", value8);
                                    break;
                                case 0x02:
                                    int16_t value16;
                                    fread(&value16, sizeof(int16_t), 1, file);

                                    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                                        value16 = swapInt16(value16);
                                    #endif

                                    printf("%d", value16);
                                    break;
                                case 0x03:
                                    int32_t value32;
                                    fread(&value32, sizeof(int32_t), 1, file);

                                    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                                        value32 = swapInt32(value32);
                                    #endif

                                    printf("%d", value32);
                                    break;
                                case 0x04:
                                    int64_t value64;
                                    fread(&value64, sizeof(int64_t), 1, file);

                                    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                                        value64 = swapInt64(value64);
                                    #endif

                                    printf("%d", value64);
                                    break;

                                case 0x05:
                                    uint32_t stringSize;
                                    fread(&stringSize, sizeof(uint32_t), 1, file);

                                    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                                        stringSize = swapUint32(stringSize);
                                    #endif

                                    char *string = malloc(stringSize + 1);
                                    if (string == NULL) {
                                        fclose(file);
                                        printf(ERRCODE_4);
                                        return 4;
                                    }

                                    fread(string, sizeof(char), stringSize, file);
                                    string[stringSize] = '\0';

                                    printf("%s", string);
                                    free(string);
                                    break;

                                default:
                                    fclose(file);
                                    printf("[calclang]: Unknown register 0x%02X! (code 5)", numberRegister);
                                    return 5;
                            }

                            fseek(file, ip, SEEK_SET);
                        } else {
                            fclose(file);
                            printf(ERRCODE_6);
                            return 6;
                        }
                    }
                    break;

                case (enum bytecodecmd)PRINTMEM:
                    {
                        uint8_t currentRegister;
                        uint32_t registerIndex;

                        fread(&currentRegister, sizeof(uint8_t), 1, file);
                        fread(&registerIndex, sizeof(uint32_t), 1, file);
                        ip += sizeof(uint8_t);
                        ip += sizeof(uint32_t);

                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                            registerIndex = swapUint32(registerIndex);
                        #endif

                        switch (currentRegister) {
                            case 0x01:
                                printf("%d", ax[registerIndex]);
                                break;
                            case 0x02:
                                printf("%d", bx[registerIndex]);
                                break;
                            case 0x03:
                                printf("%d", cx[registerIndex]);
                                break;
                            case 0x04:
                                printf("%d", dx[registerIndex]);
                                break;
                            case 0x05:
                                printf("%s", stx[registerIndex]);
                                break;

                            default:
                                fclose(file);
                                printf("[calclang]: Unknown register 0x%02X! (code 5)", currentRegister);
                                return 5;
                        }
                    }
                    break;

                case (enum bytecodecmd)ADD:
                    {
                        uint8_t firstRegisterInt;
                        uint8_t secondRegisterInt;
                        uint32_t firstRegisterIndex;
                        uint32_t secondRegisterIndex;

                        fread(&firstRegisterInt, sizeof(uint8_t), 1, file);
                        fread(&firstRegisterIndex, sizeof(uint32_t), 1, file);
                        fread(&secondRegisterInt, sizeof(uint8_t), 1, file);
                        fread(&secondRegisterIndex, sizeof(uint32_t), 1, file);
                        ip += sizeof(uint8_t) * 2;
                        ip += sizeof(uint32_t) * 2;

                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                            firstRegisterIndex = swapUint32(firstRegisterIndex);
                            secondRegisterIndex = swapUint32(secondRegisterIndex);
                        #endif

                        int exitCode = ariphmeticAction(add_op, firstRegisterInt, secondRegisterInt, firstRegisterIndex, secondRegisterIndex, file, ax, bx, cx, dx);
                        if (exitCode != 0) {
                            return exitCode;
                        }
                    }
                    break;

                case (enum bytecodecmd)SUB:
                    {
                        uint8_t firstRegisterInt;
                        uint8_t secondRegisterInt;
                        uint32_t firstRegisterIndex;
                        uint32_t secondRegisterIndex;

                        fread(&firstRegisterInt, sizeof(uint8_t), 1, file);
                        fread(&firstRegisterIndex, sizeof(uint32_t), 1, file);
                        fread(&secondRegisterInt, sizeof(uint8_t), 1, file);
                        fread(&secondRegisterIndex, sizeof(uint32_t), 1, file);
                        ip += sizeof(uint8_t) * 2;
                        ip += sizeof(uint32_t) * 2;

                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                            firstRegisterIndex = swapUint32(firstRegisterIndex);
                            secondRegisterIndex = swapUint32(secondRegisterIndex);
                        #endif

                        int exitCode = ariphmeticAction(sub_op, firstRegisterInt, secondRegisterInt, firstRegisterIndex, secondRegisterIndex, file, ax, bx, cx, dx);
                        if (exitCode != 0) {
                            return exitCode;
                        }
                    }
                    break;

                case (enum bytecodecmd)MUL:
                    {
                        uint8_t firstRegisterInt;
                        uint8_t secondRegisterInt;
                        uint32_t firstRegisterIndex;
                        uint32_t secondRegisterIndex;

                        fread(&firstRegisterInt, sizeof(uint8_t), 1, file);
                        fread(&firstRegisterIndex, sizeof(uint32_t), 1, file);
                        fread(&secondRegisterInt, sizeof(uint8_t), 1, file);
                        fread(&secondRegisterIndex, sizeof(uint32_t), 1, file);
                        ip += sizeof(uint8_t) * 2;
                        ip += sizeof(uint32_t) * 2;

                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                            firstRegisterIndex = swapUint32(firstRegisterIndex);
                            secondRegisterIndex = swapUint32(secondRegisterIndex);
                        #endif

                        int exitCode = ariphmeticAction(mul_op, firstRegisterInt, secondRegisterInt, firstRegisterIndex, secondRegisterIndex, file, ax, bx, cx, dx);
                        if (exitCode != 0) {
                            return exitCode;
                        }
                    }
                    break;

                case (enum bytecodecmd)DIV:
                    {
                        uint8_t firstRegisterInt;
                        uint8_t secondRegisterInt;
                        uint32_t firstRegisterIndex;
                        uint32_t secondRegisterIndex;

                        fread(&firstRegisterInt, sizeof(uint8_t), 1, file);
                        fread(&firstRegisterIndex, sizeof(uint32_t), 1, file);
                        fread(&secondRegisterInt, sizeof(uint8_t), 1, file);
                        fread(&secondRegisterIndex, sizeof(uint32_t), 1, file);
                        ip += sizeof(uint8_t) * 2;
                        ip += sizeof(uint32_t) * 2;

                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                            firstRegisterIndex = swapUint32(firstRegisterIndex);
                            secondRegisterIndex = swapUint32(secondRegisterIndex);
                        #endif

                        int exitCode = ariphmeticAction(div_op, firstRegisterInt, secondRegisterInt, firstRegisterIndex, secondRegisterIndex, file, ax, bx, cx, dx);
                        if (exitCode != 0) {
                            return exitCode;
                        }
                    }
                    break;

                case (enum bytecodecmd)SET:
                    {
                        uint8_t currentRegister;
                        uint32_t registerIndex;

                        fread(&currentRegister, sizeof(uint8_t), 1, file);
                        fread(&registerIndex, sizeof(uint32_t), 1, file);
                        ip += sizeof(uint8_t);
                        ip += sizeof(uint32_t);

                        #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                            registerIndex = swapUint32(registerIndex);
                        #endif

                        switch (currentRegister) {
                            case 0x01:
                                int8_t value8;
                                fread(&value8, sizeof(int8_t), 1, file);
                                ip += sizeof(int8_t);

                                ax[registerIndex] = value8;
                                break;
                            case 0x02:
                                int16_t value16;
                                fread(&value16, sizeof(int16_t), 1, file);
                                ip += sizeof(int16_t);

                                #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                                    value16 = swapInt16(value16);
                                #endif

                                bx[registerIndex] = value16;
                                break;
                            case 0x03:
                                int32_t value32;
                                fread(&value32, sizeof(int32_t), 1, file);
                                ip += sizeof(int32_t);

                                #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                                    value32 = swapInt32(value32);
                                #endif

                                cx[registerIndex] = value32;
                                break;
                            case 0x04:
                                int64_t value64;
                                fread(&value64, sizeof(int64_t), 1, file);
                                ip += sizeof(int64_t);

                                #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                                    value64 = swapInt64(value64);
                                #endif

                                dx[registerIndex] = value64;
                                break;
                            case 0x05:
                                if (stx[registerIndex] != NULL) free(stx[registerIndex]);
                                uint32_t stringLength;
                                fread(&stringLength, sizeof(uint32_t), 1, file);
                                ip += sizeof(uint32_t);

                                #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                                    stringLength = swapUint32(stringLength);
                                #endif

                                stx[registerIndex] = malloc(stringLength + 1);
                                if (stx[registerIndex] == NULL) {
                                    fclose(file);
                                    printf(ERRCODE_4);
                                    return 4;
                                }
                                fread(&stx[registerIndex], 1, stringLength, file);
                                stx[registerIndex][stringLength] = '\0';
                                ip += stringLength;

                                break;

                            default:
                                fclose(file);
                                printf("[calclang]: Unknown register 0x%02X! (code 5)", currentRegister);
                                return 5;
                        }
                    }
                    break;

                default:
                    {
                        fclose(file);
                        printf("[calclang]: Unknown opcode: 0x%02X (code 1)", opcode);
                        return 1;
                    }
                    break;
            }
        }
    }

    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    char *filename;

    if (argc > 2) {
        printf("Too many arguments!");
        return 1;
    } else if (argc == 2) {

        filename = argv[1];
    }

    int exitCode = executeBytecode(filename);

    return exitCode;
}