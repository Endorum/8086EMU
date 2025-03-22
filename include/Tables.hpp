//
// Created by Alexander on 22.03.25.
//

#ifndef TABLES_HPP
#define TABLES_HPP

#include <cstdint>
#include <string>


#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t


typedef enum OpcodeType{
    NONE,
    ERROR,
    Immed,
    Shift,
    Grp1,
    Grp2,

    // Data Transfer
    MOV,
    PUSH,
    POP,
    XCHG,
    IN,
    OUT,
    XLAT,
    LEA,
    LDS,
    LES,
    LAHF,
    SAHF,
    PUSHF,
    POPF,

    // Arithmetic
    ADD,
    ADC,
    INC,
    AAA,
    DAA,
    SUB,
    SBB,
    DEC,
    NEG,
    CMP,
    AAS,
    DAS,
    MUL,
    IMUL,
    AAM,
    DIV,
    IDIV,
    AAD,
    CBW,
    CWD,

    // Logic
    NOT,
    SHL, //(SAL)
    SHR,
    SAR,
    ROL,
    ROR,
    RCL,
    RCR,
    AND,
    TEST,
    OR,
    XOR,

    // String manipulation
    REP,
    MOVS,
    CMPS,
    SCAS,
    LODS,
    STOS,

    // Control Transfer
    CALL,
    JMP,
    RET,
    JE, //(JZ)
    JL,
    JLE,
    JB,
    JBE,
    JP,
    JO,
    JS,
    JNE,
    JNL,
    JNLE,
    JNB,
    JNBE,
    JNP,
    JNO,
    JNS,
    LOOP,
    LOOPZ,
    LOOPNZ,
    JCXZ,
    INT,
    INTO,
    IRET,

    // Processor Control
    CLC,
    CMC,
    STC,
    CLD,
    STD,
    CLI,
    STI,
    HLT,
    WAIT,
    ESC,
    LOCK,
    SEGMENT





}OpcodeType;

inline OpcodeType getOpcodeType(u8 opc, u8 modRM) {
    OpcodeType opTypeLookUp[] = {
        ADD,    ADD,    ADD,    ADD,    ADD,    ADD,    PUSH,   POP,    OR,     OR,     OR,     OR,     OR,     OR,     PUSH,   NONE,
        ADC,    ADC,    ADC,    ADC,    ADC,    ADC,    PUSH,   POP,    SBB,    SBB,    SBB,    SBB,    SBB,    SBB,    PUSH,   POP,
        AND,    AND,    AND,    AND,    AND,    AND,    SEGMENT,DAA,    SUB,    SUB,    SUB,    SUB,    SUB,    SUB,    SEGMENT,DAS,
        XOR,    XOR,    XOR,    XOR,    XOR,    XOR,    SEGMENT,AAA,    CMP,    CMP,    CMP,    CMP,    CMP,    CMP,    SEGMENT,AAS,
        INC,    INC,    INC,    INC,    INC,    INC,    INC,    INC,    DEC,    DEC,    DEC,    DEC,    DEC,    DEC,    DEC,    DEC,
        PUSH,   PUSH,   PUSH,   PUSH,   PUSH,   PUSH,   PUSH,   PUSH,   POP,    POP,    POP,    POP,    POP,    POP,    POP,    POP,
        NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,   NONE,
        JO,     JNO,    JB,     JNB,    JE,     JNE,    JBE,    JNBE,   JS,     JNS,    JP,     JNP,    JL,     JNL,    JLE,    JNLE,
        Immed,  Immed,  Immed,  Immed,  TEST,   TEST,   XCHG,   XCHG,   MOV,    MOV,    MOV,    MOV,    MOV,    LEA,    MOV,    POP,
        XCHG,   XCHG,   XCHG,   XCHG,   XCHG,   XCHG,   XCHG,   XCHG,   CBW,    CWD,    CALL,   WAIT,   PUSHF,  POPF,   SAHF,   LAHF,
        MOV,    MOV,    MOV,    MOV,    MOVS,   MOVS,   CMPS,   CMPS,   TEST,   TEST,   STOS,   STOS,   LODS,   LODS,   SCAS,   SCAS,
        MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,    MOV,
        NONE,   NONE,   RET,    RET,    LES,    LDS,    MOV,    MOV,    NONE,   NONE,   RET,    RET,    INT,    INT,    INTO,   IRET,
        Shift,  Shift,  Shift,  Shift,  AAM,    AAD,    NONE,   XLAT,   ESC,    ESC,    ESC,    ESC,    ESC,    ESC,    ESC,    ESC,
        LOOPNZ, LOOPZ,  LOOP,   JCXZ,   IN,     IN,     OUT,    OUT,    CALL,   JMP,    JMP,    JMP,    IN,     IN,     OUT,    OUT,
        LOCK,   NONE,   REP,    REP,    HLT,    CMC,    Grp1,   Grp1,   CLC,    STC,    CLI,    STI,    CLD,    STD,    NONE,    NONE
    };

    OpcodeType type = opTypeLookUp[opc];


    if(type == Immed) {
        switch (modRM) {
            default: type = ERROR; break;
            case 0x0: type = ADD; break;
            case 0x1: type = OR; break;
            case 0x2: type = ADC; break;
            case 0x3: type = SBB; break;
            case 0x4: type = AND; break;
            case 0x5: type = SUB; break;
            case 0x6: type = XOR; break;
            case 0x7: type = CMP; break;
        }
    }

    else if(type == Shift) {
        switch (modRM) {
            default: type = ERROR; break;
            case 0x0: type = ROL; break;
            case 0x1: type = ROR; break;
            case 0x2: type = RCL; break;
            case 0x3: type = RCR; break;
            case 0x4: type = SHL; break;
            case 0x5: type = SHR; break;
            case 0x6: type = NONE; break;
            case 0x7: type = SAR; break;
        }
    }

    else if(type == Grp1) {
        switch (modRM) {
            default: type = ERROR; break;
            case 0x0: type = TEST; break;
            case 0x1: type = NONE; break;
            case 0x2: type = NOT; break;
            case 0x3: type = NEG; break;
            case 0x4: type = MUL; break;
            case 0x5: type = IMUL; break;
            case 0x6: type = DIV; break;
            case 0x7: type = IDIV; break;
        }
    }

    else if(type == Grp2) {
        switch (modRM) {
            default: type = ERROR; break;
            case 0x0: type = INC; break;
            case 0x1: type = DEC; break;
            case 0x2:
            case 0x3: type = CALL; break;
            case 0x4:
            case 0x5: type = JMP; break;
            case 0x6: type = PUSH; break;
            case 0x7: type = NONE; break;
        }
    }

    return type;
}

inline std::string getOpcodeMnemonic(OpcodeType type) {
    std::string table[] = {
        "NONE",
        "ERROR",
        "Immed",
        "Shift",
        "Grp1",
        "Grp2",
        "MOV",
        "PUSH",
        "POP",
        "XCHG",
        "IN",
        "OUT",
        "XLAT",
        "LEA",
        "LDS",
        "LES",
        "LAHF",
        "SAHF",
        "PUSHF",
        "POPF",
        "ADD",
        "ADC",
        "INC",
        "AAA",
        "DAA",
        "SUB",
        "SBB",
        "DEC",
        "NEG",
        "CMP",
        "AAS",
        "DAS",
        "MUL",
        "IMUL",
        "AAM",
        "DIV",
        "IDIV",
        "AAD",
        "CBW",
        "CWD",
        "NOT",
        "SHL",
        "SHR",
        "SAR",
        "ROL",
        "ROR",
        "RCL",
        "RCR",
        "AND",
        "TEST",
        "OR",
        "XOR",
        "REP",
        "MOVS",
        "CMPS",
        "SCAS",
        "LODS",
        "STOS",
        "CALL",
        "JMP",
        "RET",
        "JE",
        "JL",
        "JLE",
        "JB",
        "JBE",
        "JP",
        "JO",
        "JS",
        "JNE",
        "JNL",
        "JNLE",
        "JNB",
        "JNBE",
        "JNP",
        "JNO",
        "JNS",
        "LOOP",
        "LOOPZ",
        "LOOPNZ",
        "JCXZ",
        "INT",
        "INTO",
        "IRET",
        "CLC",
        "CMC",
        "STC",
        "CLD",
        "STD",
        "CLI",
        "STI",
        "HLT",
        "WAIT",
        "ESC",
        "LOCK",
        "SEGMENT"
    };

    return table[type];
}
#endif //TABLES_HPP
