#ifndef SWAPVALUE_H
#define SWAPVALUE_H

#include <stdint.h>

// #undef _WIN64
// #define __linux__

#if defined(_WIN64)
    extern int16_t __asm_win64_swapInt16(int16_t value);
    extern int32_t __asm_win64_swapInt32(int32_t value);
    extern uint32_t __asm_win64_swapUint32(uint32_t value);
    extern int64_t __asm_win64_swapInt64(int64_t value);

    static inline int16_t swapInt16(int16_t value) {
        return __asm_win64_swapInt16(value);
    }
    static inline int32_t swapInt32(int32_t value) {
        return __asm_win64_swapInt32(value);
    }
    static inline uint32_t swapUint32(uint32_t value) {
        return __asm_win64_swapUint32(value);
    }
    static inline int64_t swapInt64(int64_t value) {
        return __asm_win64_swapInt64(value);
    }
#elif defined(__linux__)
    extern int16_t __asm_unix64_swapInt16(int16_t value);
    extern int32_t __asm_unix64_swapInt32(int32_t value);
    extern uint32_t __asm_unix64_swapUint32(uint32_t value);
    extern int64_t __asm_unix64_swapInt64(int64_t value);

    static inline int16_t swapInt16(int16_t value) {
        return __asm_unix64_swapInt16(value);
    }
    static inline int32_t swapInt32(int32_t value) {
        return __asm_unix64_swapInt32(value);
    }
    static inline uint32_t swapUint32(uint32_t value) {
        return __asm_unix64_swapUint32(value);
    }
    static inline int64_t swapInt64(int64_t value) {
        return __asm_unix64_swapInt64(value);
    }
#endif

#endif