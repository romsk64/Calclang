#define VERSION_VM "0.0"

enum bytecodecmd {
    PRINT = 0x01,
    PRINTMEM = 0x02,
    INPUT = 0x03,
    DUP = 0x04,
    ADD = 0x05,
    SUB = 0x06,
    MUL = 0x07, // умножение
    DIV = 0x08, // деление
    SET = 0x09,

    FILEEND = 0xff
};