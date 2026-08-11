; NASM, под линукс

global __asm_unix64_swapInt16
global __asm_unix64_swapInt32
global __asm_unix64_swapUint32
global __asm_unix64_swapInt64

section .text
__asm_unix64_swapInt16:
    mov ax, di
    xchg ah, al
    ret

__asm_unix64_swapInt32:
    mov eax, edi
    bswap eax
    ret

__asm_unix64_swapUint32:
    mov eax, edi
    bswap eax
    ret

__asm_unix64_swapInt64:
    mov rax, rdi
    bswap rax
    ret