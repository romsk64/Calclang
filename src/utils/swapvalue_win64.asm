; NASM, под Win64

global __asm_win64_swapInt16
global __asm_win64_swapInt32
global __asm_win64_swapUint32
global __asm_win64_swapInt64

section .text
__asm_win64_swapInt16:
    mov ax, cx
    xchg ah, al
    ret

__asm_win64_swapInt32:
    mov eax, ecx
    bswap eax
    ret

__asm_win64_swapUint32:
    mov eax, ecx
    bswap eax
    ret

__asm_win64_swapInt64:
    mov rax, rcx
    bswap rax
    ret