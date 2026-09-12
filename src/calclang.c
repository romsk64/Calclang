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

/*
 * PUSH
 * POP
 * ADD
 * SUB
 * MUL
 * DIV
 * DIVMDXPOW2 - деление с операндом во второй степени
*/

typedef struct CLVMStack {
    uint64_t *stack;
    uint64_t *stackTopPtr;
} CLVMStack;

/*int8_t stackPop_int8(CLVMStack *stack) {
    int8_t result = *(int8_t *)(stack->stackTopPtr);
    (stack->stackTopPtr)--;

    return result;
}
uint8_t stackPop_uint8(CLVMStack *stack) {
    uint8_t result = *(uint8_t *)(stack->stackTopPtr);
    (stack->stackTopPtr)--;

    return result;
}
int16_t stackPop_int16(CLVMStack *stack) {
    int16_t result = *(int16_t *)(stack->stackTopPtr);
    (stack->stackTopPtr) -= 2;

    return result;
}
uint16_t stackPop_uint16(CLVMStack *stack) {
    uint16_t result = *(uint16_t *)(stack->stackTopPtr);
    (stack->stackTopPtr) -= 2;

    return result;
}
int32_t stackPop_int32(CLVMStack *stack) {
    int32_t result = *(int32_t *)(stack->stackTopPtr);
    (stack->stackTopPtr) -= 4;

    return result;
}
uint32_t stackPop_uint32(CLVMStack *stack) {
    uint32_t result = *(uint32_t *)(stack->stackTopPtr);
    (stack->stackTopPtr) -= 4;

    return result;
}*/
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
/*
void stackPush_int8(CLVMStack *stack, int8_t value) {
    int8_t *valuePtr = (int8_t *)(stack->stackTopPtr);
}*/

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

int executeBytecode(const char *restrict const filename, uint64_t stackSize) {

int main(int argc, char *argv[]) {
    char *filename;

    if (argc == 1) {
        printf("Calclang Virtual Machine (v.%s)\n", VERSION_VM);
        printf("Using:\n");
        printf("\tcalclang {bytecode filename}\n");
        return 0;
    } else if (argc > 2) {
        printf("Too many arguments!");
        return 1;
    } else if (argc == 2) {

        filename = argv[1];
    }

    int exitCode = executeBytecode(filename);

    return exitCode;
}
