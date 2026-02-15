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

#define s8 int8_t
#define s16 int16_t
#define s32 int32_t


typedef enum DataType {
    DT_modRegRM,
    DT_DATA8,
    DT_DATA_SX,
    DT_DATA_LO,
    DT_DATA_HI,
    DT_DISP_LO,
    DT_DISP_HI,
    DT_IP_LO,
    DT_IP_HI,
    DT_CS_LO,
    DT_CS_HI,
    DT_IP_INC8,
    DT_IP_INC_LO,
    DT_IP_INC_HI,
    DT_ADDR_LO,
    DT_ADDR_HI,
    DT_XXX,
    DT_YYY,
    DT_IMMED8,
    DT_SEGREG,
    DT_SEG_LO,
    DT_SEG_HI,
    DT_DEST_STR8,
    DT_none,
}DataType;

typedef enum RegType {
    NOREG,
    AH, AL, AX,
    BH, BL, BX,
    CH, CL, CX,
    DH, DL, DX,
    SP, BP,
    SI, DI,
    IP
}RegType;

inline std::string getRegTypeName(RegType type) {
    if(type == NOREG) return "NOREG";
    if(type == AH) return "AH";
    if(type == AL) return "AL";
    if(type == AX) return "AX";
    if(type == BH) return "BH";
    if(type == BL) return "BL";
    if(type == BX) return "BX";
    if(type == CH) return "CH";
    if(type == CL) return "CL";
    if(type == CX) return "CX";
    if(type == DH) return "DH";
    if(type == DL) return "DL";
    if(type == DX) return "DX";
    if(type == SP) return "SP";
    if(type == BP) return "BP";
    if(type == SI) return "SI";
    if(type == DI) return "DI";
    if(type == IP) return "IP";
    return "";
}


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
    SEGMENT,


    SEG_ES,
    SEG_SS,
    SEG_CS,
    SEG_DS,




}OpcodeType;

inline OpcodeType getOpcodeType(u8 opc, u8 modRM) {
    OpcodeType opTypeLookUp[] = {
        ADD,    ADD,    ADD,    ADD,    ADD,    ADD,    PUSH,   POP,    OR,     OR,     OR,     OR,     OR,     OR,     PUSH,   NONE,
        ADC,    ADC,    ADC,    ADC,    ADC,    ADC,    PUSH,   POP,    SBB,    SBB,    SBB,    SBB,    SBB,    SBB,    PUSH,   POP,
        AND,    AND,    AND,    AND,    AND,    AND,    SEG_ES, DAA,    SUB,    SUB,    SUB,    SUB,    SUB,    SUB,    SEG_CS, DAS,
        XOR,    XOR,    XOR,    XOR,    XOR,    XOR,    SEG_SS, AAA,    CMP,    CMP,    CMP,    CMP,    CMP,    CMP,    SEG_DS,AAS,
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
        u8 Reg = (modRM & 0x38) >> 3;
        switch (Reg) {
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
        u8 Reg = (modRM & 0x38) >> 3;
        switch (Reg) {
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
        u8 Reg = (modRM & 0x38) >> 3;
        switch (Reg) {
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
        u8 Reg = (modRM & 0x38) >> 3;
        switch (Reg) {
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

typedef enum AddrCode {
    _0,     // none
    A,      // Direct address. The instruction has no ModR/M byte; the address of the operand is encoded in the instruction. Applicable, e.g., to far JMP (opcode EA).
    E,      // A ModR/M byte follows the opcode and specifies the operand. The operand is either a general-purpose register or a memory address. If it is a memory address, the address is computed from a segment register and any of the following values: a base register, an index register, a displacement.
    G,      // The reg field of the ModR/M byte selects a general register.
    I,      // Immediate data. The operand value is encoded in subsequent bytes of the instruction.
    J,      // The instruction contains a relative offset to be added to the address of the subsequent instruction. Applicable, e.g., to short JMP (opcode EB), or LOOP.
    M,      // The ModR/M byte may refer only to memory. Applicable, e.g., to LES and LDS.
    O,      // The instruction has no ModR/M byte; the offset of the operand is encoded as a WORD in the instruction. Applicable, e.g., to certain MOVs (opcodes A0 through A3).
    S,      // The reg field of the ModR/M byte selects a segment register.

    b,      // Byte argument.
    p,      // 32-bit segment:offset pointer.
    w,      // Word argument
    v,      // Word argument. (The 'v' code has a more complex meaning in later x86 opcode maps, from which this was derived, but here it's just a synonym for the 'w' code.)

    AH_, AL_, AX_,
    BH_, BL_, BX_,
    CH_, CL_, CX_,
    DH_, DL_, DX_,
    SP_, BP_,
    SI_, DI_,
    IP_,

    ES_, CS_, SS_, DS_,

    _1, // A constant argument of 1, implicit in the opcode, and not represented elsewhere in the instruction. This argument *is* displayed in assembly code.
    _3, // A constant argument of 3, implicit in the opcode, and not represented elsewhere in the instruction. This argument *is* displayed in assembly code.


}AddrCode;

inline AddrCode getArgAddrCodeFirst(u8 opc) {
    AddrCode table[256] = {
        E, E, G, G, AL_, AX_, ES_, ES_, E, E, G, G, AL_, AX_, CS_, _0,
        E, E, G, G, AL_, AX_, SS_, SS_, E, E, G, G, AL_, AX_, DS_, DS_,
        E, E, G, G, AL_, AX_,  _0,  _0, E, E, G, G, AL_, AX_,  _0,  _0,
        E, E, G, G, AL_, AX_,  _0,  _0, E, E, G, G, AL_, AX_,  _0,  _0,
        AX_, CX_, DX_, BX_, SP_, BP_, SI_, DI_, AX_, CX_, DX_, BX_, SP_, BP_, SI_, DI_,
        AX_, CX_, DX_, BX_, SP_, BP_, SI_, DI_, AX_, CX_, DX_, BX_, SP_, BP_, SI_, DI_,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        J, J, J, J, J, J, J, J, J, J, J, J, J, J, J, J,
        E, E, E, E, G, G, G, G, E, E, G, G, E, G, S, E,
        _0, CX_, DX_, BX_, SP_, BP_, SI_, DI_, _0, _0, A, _0, _0, _0, _0, _0,
        AL_, AX_, O, O, _0, _0, _0, _0, A, A, _0, _0, _0, _0, _0, _0,
        AL_, CL_, DL_, BL_, AH_, CH_, DH_, BH_, AX_, CX_, DX_, BX_, SP_, BP_, SI_, DI_,
        _0, _0, I, _0, G, G, E, E, _0, _0, I, _3, I, _0, _0,
        E, E, E, E, I, I, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        J, J, J, J, AL_, AX_, I, I, J, J, A, J, AL_, AX_, DX_, DX_,
        _0, _0, _0, _0, _0, _0, E, E, _0, _0, _0, _0, _0, _0, E, E,
    };

    return table[opc];
}

inline AddrCode getArgAddrCodeSecond(u8 opc) {
    AddrCode table[256] = {
        G, G, E, E, I, I, _0, _0, G, G, E, E, I, I, _0, _0,
        G, G, E, E, I, I, _0, _0, G, G, E, E, I, I, _0, _0,
        G, G, E, E, I, I, _0, _0, G, G, E, E, I, I, _0, _0,
        G, G, E, E, I, I, _0, _0, G, G, E, E, I, I, _0, _0,

        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,

        I, I, I, I, E, E, E, E, G, G, E, E, S, M, E, _0,
        _0, AX_, AX_, AX_, AX_, AX_, AX_, AX_, _0, _0, _0, _0, _0, _0, _0, _0,
        O, O, AL_, AX_, _0, _0, _0, _0, I, I, _0, _0, _0, _0, _0, _0,
        I, I, I, I, I, I, I, I, I, I, I, I, I, I, I, I,

        _0, _0, _0, _0, M, M, I, I, _0, _0, _0, _0, _0, _0, _0, _0,
        _1, _1, CL_, CL_, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, I, I, AL_, AX_, _0, _0, _0, _0, DX_, DX_, AL_, AX_,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
    };

    return table[opc];
}

inline AddrCode getArgOperandCodeFirst(u8 opc) {
    AddrCode table[256] = {
        b, v, b, v, _0, _0, _0, _0, b, v, b, v, _0, _0, _0, _0,
        b, v, b, v, _0, _0, _0, _0, b, v, b, v, _0, _0, _0, _0,
        b, v, b, v, _0, _0, _0, _0, b, v, b, v, _0, _0, _0, _0,
        b, v, b, v, _0, _0, _0, _0, b, v, b, v, _0, _0, _0, _0,

        _0, _0, _0, _0,_0, _0, _0, _0, _0, _0, _0, _0,_0, _0, _0, _0,
        _0, _0, _0, _0,_0, _0, _0, _0, _0, _0, _0, _0,_0, _0, _0, _0,
        _0, _0, _0, _0,_0, _0, _0, _0, _0, _0, _0, _0,_0, _0, _0, _0,
        b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b,

        b, v, b, v, b, v, b, v,  b, v, b, v, w, v, w, v,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, p, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,

        _0, _0, w, _0, v, v, b, v, _0, _0, w, _0, _0, b, _0, _0,
        b, v, b, v, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        b, b, b, b, _0, _0, b, b, v, v, p, b, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, b, v, _0, _0, _0, _0, _0, _0, b, v,

    };

    return table[opc];
}

inline AddrCode getArgOperandCodeSecond(u8 opc) {
    AddrCode table[256] = {
        b, v, b, v, b, v, _0, _0,  b, v, b, v, b, v, _0, _0,
        b, v, b, v, b, v, _0, _0,  b, v, b, v, b, v, _0, _0,
        b, v, b, v, b, v, _0, _0,  b, v, b, v, b, v, _0, _0,
        b, v, b, v, b, v, _0, _0,  b, v, b, v, b, v, _0, _0,

        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,

        b, v, b, b, b, v, b, v,  b, v, b, v, w, _0, w, v,
        _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0, _0,
        b, v, _0, _0, _0, _0, _0, _0, b, v, _0, _0, _0, _0, _0, _0,
        b, b, b, b, b, b, b, b, v, v, v, v, v, v, v, v,

        _0, _0, _0, _0, p, p, b, v,  _0, _0, _0, _0, _0, _0, _0, _0,
        _1, _1, _0, _0, _0, _0, _0, _0,  _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, b, b, _0, _0,  _0, _0, _0, _0, _0, _0, _0, _0,
        _0, _0, _0, _0, _0, _0, _0, _0,  _0, _0, _0, _0, _0, _0, _0, _0,

    };

    return table[opc];
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
