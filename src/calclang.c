/** Виртуальная машина Calclang
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "utils/fexists.h"
#include "utils/swapvalue.h"
#include "bytecode.h"

#define ERRCODE_1 "[calclang]: Incorrect sintax of running parameters! (code 1)"
#define ERRCODE_2 "[calclang]: Incorrect signature of executable file! (code 2)"
#define ERRCODE_3 "[calclang]: Error of reading file! (code 3)"
#define ERRCODE_4 "[calclang]: Error of memory allocation! (code 4)"
#define ERRCODE_6 "[calclang]: Address error! (code 6)"
#define ERRCODE_7 "[calclang]: Opcode don't supporting register! (code 7)"
#define ERRCODE_8 "[calclang]: Metadata error! (code 8)"

// METADATA_LENGTH = 4 + (sizeof(uint32_t) * 2) + sizeof(uint8_t)
#define METADATA_LENGTH 13
#define INT32_SIZE sizeof(int32_t)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define __ORDER_LITTLE_ENDIAN_DEFINED__
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define __ORDER_BIG_ENDIAN_DEFINED__
#else
    #error "Unsupported byte order!"
#endif

/*
 * PUSH
 * POP
 * ADD
 * SUB
 * MUL
 * DIV
*/

typedef struct CLVMStack {
    uint64_t *stack;
    uint64_t *stackTopPtr;
} CLVMStack;

int64_t stackPop_int64(CLVMStack *stack) {
    int64_t result = *(int64_t *)(stack->stackTopPtr);
    (stack->stackTopPtr) -= 1;

    return result;
}
uint64_t stackPop_uint64(CLVMStack *stack) {
    uint64_t result = *(uint64_t *)(stack->stackTopPtr);
    (stack->stackTopPtr) -= 1;

    return result;
}

void stackPush_int64(CLVMStack *stack, int64_t value) {
    (stack->stackTopPtr) += 1;
    *stackTopPtr = value;
}
void stackPush_uint64(CLVMStack *stack, uint64_t value) {
    (stack->stackTopPtr) += 1;
    *stackTopPtr = value;
}

int ariphmeticAction_signed(int64_t (*function)(int64_t, int64_t), uint16_t numberBitsCount, CLVMStack *stack) {
    int64_t num1;
    int64_t num2;

    switch (numberBitsCount) {
        case 8:
            num1 = (int8_t)stackPop_int64(stack);
            num2 = (int8_t)stackPop_int64(stack);

            stackPush_int64(function(num1, num2));
            break;

        case 16:
            num1 = (int16_t)stackPop_int64(stack);
            num2 = (int16_t)stackPop_int64(stack);

            stackPush_int64(function(num1, num2));
            break;

        case 32:
            num1 = (int32_t)stackPop_int64(stack);
            num2 = (int32_t)stackPop_int64(stack);

            stackPush_int64(function(num1, num2));
            break;

        case 64:
            num1 = (int64_t)stackPop_int64(stack);
            num2 = (int64_t)stackPop_int64(stack);

            stackPush_int64(function(num1, num2));
            break;

        default:
            fprintf(stderr, ERRCODE_7);
            return 7;
    }

    return 0;
}

int ariphmeticAction_unsigned(uint64_t (*function)(uint64_t, uint64_t), uint16_t numberBitCount, CLVMStack *stack) {
    uint64_t num1;
    uint64_t num2;

    switch (numberBitCount) {
        case 8:
            num1 = (uint8_t)stackPop_uint64(stack);
            num2 = (uint8_t)stackPop_uint64(stack);

            stackPush_uint64(function(num1, num2));
            break;

        case 16:
            num1 = (uint16_t)stackPop_uint64(stack);
            num2 = (uint16_t)stackPop_uint64(stack);

            stackPush_uint64(function(num1, num2));
            break;

        case 32:
            num1 = (uint32_t)stackPop_uint64(stack);
            num2 = (uint32_t)stackPop_uint64(stack);

            stackPush_uint64(function(num1, num2));
            break;

        case 64:
            num1 = (uint64_t)stackPop_uint64(stack);
            num2 = (uint64_t)stackPop_uint64(stack);

            stackPush_uint64(function(num1, num2));
            break;

        default:
            fprintf(stderr, ERRCODE_7);
            return 7;
    }

    return 0;
}

int64_t addsigned_op(int64_t x, int64_t y) {
    return (x + y);
}
int64_t subsigned_op(int64_t x, int64_t y) {
    return (x - y);
}
int64_t mulsigned_op(int64_t x, int64_t y) {
    return (x * y);
}
int64_t divsigned_op(int64_t x, int64_t y) {
    if (y == 0) return 0;
    return (x / y);
}

uint64_t addunsigned_op(uint64_t x, uint64_t y) {
    return (x + y);
}
uint64_t subunsigned_op(uint64_t x, uint64_t y) {
    return (x - y);
}
uint64_t mulunsigned_op(uint64_t x, uint64_t y) {
    return (x * y);
}
uint64_t divunsigned_op(uint64_t x, uint64_t y) {
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

int executeBytecode(FILE *restrict file, uint64_t stackSize) {
    // доделать потом

    return 0;
}

int main(int argc, char *argv[]) {
    char *filename;
    uint64_t stackSize = STD_STACKSIZE;

    if (argc == 1) {
        printf("Calclang Virtual Machine (v.%s)\n", VERSION_VM);
        printf("Using:\n");
        printf("\tcalclang <bytecode filename>\n");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-ss") == 0) { /* stack size */
            if ((i + 1) >= argc) {
                fprintf(stderr, ERRCODE_1);
                return 1;
            }

            i++;

            if (strcmp

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, ERRCODE_3);
        return 3;
    }

    int exitCode = executeBytecode(file, stackSize);

    return exitCode;
}
