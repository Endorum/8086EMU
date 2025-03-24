//
// Created by Alexander on 22.03.25.
//

#include "../include/decoder.hpp"
#include "../include/CPU.hpp" // Now we can safely include CPU.hpp



uint8_t Decoder::readNext() {
    return cpu->readNext();
}



uint8_t Decoder::decodeModRM(uint8_t modRM) {
    u8 Mod = (modRM & 0xC0) >> 6;  // Extract Mod bits (bits 6-7)
    u8 Reg = (modRM & 0x38) >> 3;  // Extract Reg bits (bits 3-5)
    u8 RM  = (modRM & 0x07);       // Extract RM bits (bits 0-2)

    current.mod = Mod;
    current.reg = Reg;
    current.rm = RM;

    // If Mod == 00 and RM == 110, we have a direct address (16-bit displacement)
    if (Mod == 0b00 && RM == 0b110) {
        current.ADDR_LO = readNext();
        current.ADDR_HI = readNext();
    }
    // If Mod == 01 or 10, read displacement bytes
    else if (Mod == 0b01) {
        current.DISP_LO = readNext(); // 8-bit displacement
    }
    else if (Mod == 0b10) {
        current.DISP_LO = readNext(); // 16-bit displacement (LO)
        current.DISP_HI = readNext(); // 16-bit displacement (HI)
    }

    // Register lookup table for REG and RM fields
    static constexpr RegType regTableW[8] = {AX, CX, DX, BX, SP, BP, SI, DI};
    static constexpr RegType regTableB[8] = {AL, CL, DL, BL, AH, CH, DH, BH};

    current.regA = NOREG;
    current.regB = NOREG;

    // Determine if we are using word (w = 1) or byte (w = 0) registers
    current.regA = (current.w) ? regTableW[Reg] : regTableB[Reg];

    if (Mod == 0b11) {
        // Direct register mode
        current.regB = (current.w) ? regTableW[RM] : regTableB[RM];
    }

    return 0;
}


u16* Decoder::regReferenceByType(RegType type) {
    switch (type) {
        default: return nullptr;
        case NOREG: return nullptr;
        case AH: case AL: case AX:
            return &(cpu->regs.ax);
        case BH: case BL: case BX:
            return &(cpu->regs.bx);
        case CH: case CL: case CX:
            return &(cpu->regs.cx);
        case DH: case DL: case DX:
            return &(cpu->regs.dx);
        case SP:
            return &(cpu->regs.sp);
        case BP:
            return &(cpu->regs.bp);
        case SI:
            return &(cpu->regs.si);
        case DI:
            return &(cpu->regs.di);
        case IP:
            return &(cpu->regs.ip);



    }
}

DataType Decoder::getDataType(uint8_t *ptr) {
    if(ptr == &current.modRegRM) return DT_modRegRM;
    if(ptr == &current.DATA8) return DT_DATA8;
    if(ptr == &current.DATA_SX) return DT_DATA_SX;
    if(ptr == &current.DATA_LO) return DT_DATA_LO;
    if(ptr == &current.DATA_HI) return DT_DATA_HI;
    if(ptr == &current.DISP_LO) return DT_DISP_LO;
    if(ptr == &current.DISP_HI) return DT_DISP_HI;
    if(ptr == &current.IP_LO) return DT_IP_LO;
    if(ptr == &current.IP_HI) return DT_IP_HI;
    if(ptr == &current.CS_LO) return DT_CS_LO;
    if(ptr == &current.CS_HI) return DT_CS_HI;
    if(ptr == &current.IP_INC8) return DT_IP_INC8;
    if(ptr == &current.IP_INC_LO) return DT_IP_INC_LO;
    if(ptr == &current.IP_INC_HI) return DT_IP_INC_HI;
    if(ptr == &current.ADDR_LO) return DT_ADDR_LO;
    if(ptr == &current.ADDR_HI) return DT_ADDR_HI;
    if(ptr == &current.XXX) return DT_XXX;
    if(ptr == &current.YYY) return DT_YYY;
    if(ptr == &current.IMMED8) return DT_IMMED8;
    if(ptr == &current.SEGREG) return DT_SEGREG;
    if(ptr == &current.SEG_LO) return DT_SEG_LO;
    if(ptr == &current.SEG_HI) return DT_SEG_HI;
    if(ptr == &current.DEST_STR8) return DT_DEST_STR8;
    if(ptr == &current.none) return DT_none;
    return DT_none;
}


uint8_t Decoder::decode(u16 ip) {
    current = {0};
    current.addr = ip;

    u8 opc = readNext();

    current.opcode = opc;
    current.w = (opc & 0x01) > 0;
    current.d = (opc & 0x02) > 0;

    u8* firstByte[256] = {
        &current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,
        &current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,
        &current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,
        &current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,

        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,
        &current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.DATA8,&current.DATA_LO,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DISP_LO,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.ADDR_LO,&current.ADDR_LO,&current.ADDR_LO,&current.ADDR_LO,&current.none,&current.none,&current.none,&current.none,&current.DATA8,&current.DATA_LO,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.DATA8,&current.DATA8,&current.DATA8,&current.DATA8,&current.DATA8,&current.DATA8,&current.DATA8,&current.DATA8,&current.DATA_LO,&current.DATA_LO,&current.DATA_LO,&current.DATA_LO,&current.DATA_LO,&current.DATA_LO,&current.DATA_LO,&current.DATA_LO,
        &current.none,&current.none,&current.DATA_LO,&current.none,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.none,&current.none,&current.DATA_LO,&current.none,&current.none,&current.DATA8,&current.none,&current.none,
        &current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.none,&current.none,&current.none,&current.none,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,&current.modRegRM,
        &current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.IP_INC8,&current.DATA8,&current.DATA8,&current.DATA8,&current.DATA8,&current.IP_INC_LO,&current.IP_INC_LO,&current.IP_LO,&current.IP_INC8,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.modRegRM,&current.modRegRM,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.modRegRM,&current.modRegRM,
    };

    u8* firstBytePtr = firstByte[opc];
    current.firstBytePtr = firstBytePtr;

    if(firstBytePtr != &current.none) {
        *firstByte[opc] = readNext();
    }

    if(firstBytePtr == &current.modRegRM) {
        decodeModRM(current.modRegRM);
    }

    u8* secondByte[256] = {
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,

        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,

        &current.DATA8,&current.DATA_LO,&current.DATA8,&current.DATA_SX,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DISP_HI,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.ADDR_HI,&current.ADDR_HI,&current.ADDR_HI,&current.ADDR_HI,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.DATA_HI,&current.DATA_HI,&current.DATA_HI,&current.DATA_HI,&current.DATA_HI,&current.DATA_HI,&current.DATA_HI,

        &current.none,&current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.DATA8,&current.DATA_LO,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.IP_INC_HI,&current.IP_INC_HI,&current.IP_HI,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA8,(current.reg == 0b000) ? &current.DATA_LO : &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,

    };

    u8* secondBytePtr = secondByte[opc];
    current.secondBytePtr = secondBytePtr;

    if(secondBytePtr != &current.none) {
        *secondByte[opc] = readNext();
    }

    u8* thirdByte[256] = {
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,

        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,

        &current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.SEG_LO,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,

        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.DATA_HI,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.CS_LO,&current.none,&current.none,&current.none,&current.none,&current.none,
        &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,(current.reg == 0b000) ? &current.DATA_HI : &current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,&current.none,

    };

    u8* thirdBytePtr = thirdByte[opc];
    current.thirdBytePtr = thirdBytePtr;

    if(thirdBytePtr != &current.none) {
        *thirdByte[opc] = readNext();
    }

    u8* fourthBytePtr = &current.none;

    if(current.opcode == 0x9A) {
        fourthBytePtr = &current.SEG_HI;
        current.SEG_HI = readNext();
    }else if(current.opcode == 0xEA) {
        fourthBytePtr = &current.CS_HI;
        current.CS_HI = readNext();
    }

    current.fourthBytePtr = fourthBytePtr;

    if(current.opcode >= 0xD8 && current.opcode < 0xE0) {
        current.XXX = current.opcode & 0x07;
        current.YYY = (current.modRegRM & 0x38) >> 2;
    }

    if(current.hasModRM == false) switch (current.reg) {
        default: current.type = ERROR; return 0xFF;
        case 0b000:
            current.regA = (current.w) ? AX : AL;
        break;
        case 0b001:
            current.regA = (current.w) ? CX : CL;
        break;
        case 0b010:
            current.regA = (current.w) ? DX : DL;
        break;
        case 0b011:
            current.regA = (current.w) ? BX : BL;
        break;
        case 0b100:
            current.regA = (current.w) ? SP : AH;
        break;
        case 0b101:
            current.regA = (current.w) ? BP : CH;
        break;
        case 0b110:
            current.regA = (current.w) ? SI : DH;
        break;
        case 0b111:
            current.regA = (current.w) ? DI : BH;
        break;

    }

    if(current.opcode >= 0xB0 && current.opcode < 0xC0) {
        switch (current.opcode & 0x0F) {
            default: current.opcode = ERROR; return 0xFF;
            case 0x0: current.regA = AL; break;
            case 0x1: current.regA = CL; break;
            case 0x2: current.regA = DL; break;
            case 0x3: current.regA = BL; break;
            case 0x4: current.regA = AH; break;
            case 0x5: current.regA = CH; break;
            case 0x6: current.regA = DH; break;
            case 0x7: current.regA = BH; break;
            case 0x8: current.regA = AX; break;
            case 0x9: current.regA = CX; break;
            case 0xA: current.regA = DX; break;
            case 0xB: current.regA = BX; break;
            case 0xC: current.regA = SP; break;
            case 0xD: current.regA = BP; break;
            case 0xE: current.regA = SI; break;
            case 0xF: current.regA = DI; break;

        }
    }

    current.type = getOpcodeType(opc, current.modRegRM);


    current.firstType = getDataType(current.firstBytePtr);
    current.secondType = getDataType(current.secondBytePtr);
    current.thirdType = getDataType(current.thirdBytePtr);
    current.fourthType = getDataType(current.fourthBytePtr);



    printInstruction();
    return 0;
}




NewInstruction Decoder::fillNewInstruction() {

    NewInstruction res;

    u8 opc = readNext();

    res.opcType = getOpcodeType(opc, readNext()); cpu->regs.ip--;

    res.ArgAddr0 = getArgAddrCodeFirst(opc);
    res.ArgOp0 = getArgOperandCodeFirst(opc);

    res.ArgAddr1 = getArgAddrCodeSecond(opc);
    res.ArgOp1 = getArgOperandCodeSecond(opc);


    cpu->regs.ip--;

}


std::string interpretMod(u8 Mod) {
    if(Mod == 0b00) return "Memory mode, no displacement follows (except when R/M = 110 then 16-bit displacement follows)  ";
    if(Mod == 0b01) return "Memory mode, 8-bit displacement follows                                                        ";
    if(Mod == 0b10) return "Memory mode, 16-bit displacement follows                                                       ";
    if(Mod == 0b11) return "Register mode, no displacement follows                                                         ";
    return "";
}

std::string interpretReg(u8 Reg, bool w) {
    if(Reg == 0b000) return (w) ? "AX" : "AL";
    if(Reg == 0b001) return (w) ? "CX" : "CL";
    if(Reg == 0b010) return (w) ? "DX" : "DL";
    if(Reg == 0b011) return (w) ? "BX" : "BL";
    if(Reg == 0b100) return (w) ? "SP" : "AH";
    if(Reg == 0b101) return (w) ? "BP" : "CH";
    if(Reg == 0b110) return (w) ? "SI" : "DH";
    return (w) ? "DI" : "BH";
}

std::string interpretRM(u8 RM, u8 Mod, bool w) {
    // | R/M | MOD=00         | MOD=01         | MOD=10          |
    // |-----|----------------|----------------|-----------------|
    // | 000 | (BX)+(SI)      | (BX)+(SI) + D8 | (BX)+(SI) + D16 |
    // | 001 | (BX)+(DI)      | (BX)+(DI) + D8 | (BX)+(DI) + D16 |
    // | 010 | (BP)+(SI)      | (BP)+(SI) + D8 | (BP)+(SI) + D16 |
    // | 011 | (BP)+(DI)      | (BP)+(DI) + D8 | (BP)+(DI) + D16 |
    // | 100 | (SI)           | (SI) + D8      | (SI) + D16      |
    // | 101 | (DI)           | (DI) + D8      | (DI) + D16      |
    // | 110 | DIRECT ADDRESS | (BP) + D8      | (BP) + D16      |
    // | 111 | (BX)           | (BX) + D8      | (BX) + D16      |

    std::string res;

    if(Mod == 0b11) {
        return interpretReg(RM, w);
    }

    if(RM == 0b000) {
        res = "(BX)+(SI)";
    }
    if(RM == 0b001) {
        res = "(BX)+(DI)";
    }
    if(RM == 0b010) {
        res = "(BP)+(SI)";
    }
    if(RM == 0b011) {
        res = "(BP)+(DI)";
    }
    if(RM == 0b100) {
        res = "(SI)";
    }
    if(RM == 0b101) {
        res = "(DI)";
    }
    if(RM == 0b110) {
        res = (Mod == 0b00) ? "DIRECT ADDRESS" : "(BP)";
    }
    if(RM == 0b111) {
        res = "(BX)";
    }

    if(Mod == 0b01) {
        res += "disp8";
    }
    if(Mod == 0b10) {
        res += "disp16";
    }

    return res;
}

std::string Decoder::getDataName(u8* ptr) const {
    if(ptr == &current.modRegRM) return "modRegRM";
    if(ptr == &current.DATA8) return "DATA8";
    if(ptr == &current.DATA_SX) return "DATA_SX";
    if(ptr == &current.DATA_LO) return "DATA_LO";
    if(ptr == &current.DATA_HI) return "DATA_HI";
    if(ptr == &current.DISP_LO) return "DISP_LO";
    if(ptr == &current.DISP_HI) return "DISP_HI";
    if(ptr == &current.IP_LO) return "IP_LO";
    if(ptr == &current.IP_HI) return "IP_HI";
    if(ptr == &current.CS_LO) return "CS_LO";
    if(ptr == &current.CS_HI) return "CS_HI";
    if(ptr == &current.IP_INC8) return "IP_INC8";
    if(ptr == &current.IP_INC_LO) return "IP_INC_LO";
    if(ptr == &current.IP_INC_HI) return "IP_INC_HI";
    if(ptr == &current.ADDR_LO) return "ADDR_LO";
    if(ptr == &current.ADDR_HI) return "ADDR_HI";
    if(ptr == &current.XXX) return "XXX";
    if(ptr == &current.YYY) return "YYY";
    if(ptr == &current.IMMED8) return "IMMED8";
    if(ptr == &current.SEGREG) return "SEGREG";
    if(ptr == &current.SEG_LO) return "SEG_LO";
    if(ptr == &current.SEG_HI) return "SEG_HI";
    if(ptr == &current.DEST_STR8) return "DEST_STR8";
    return "none";
}

u8 Decoder::getValue(DataType dtype) const {
    switch (dtype) {
        default: return 0xFF;
        case DT_modRegRM: return current.modRegRM;
        case DT_DATA8: return current.DATA8;
        case DT_DATA_SX: return current.DATA_SX;
        case DT_DATA_LO: return current.DATA_LO;
        case DT_DATA_HI: return current.DATA_HI;
        case DT_DISP_LO: return current.DISP_LO;
        case DT_DISP_HI: return current.DISP_HI;
        case DT_IP_LO: return current.IP_LO;
        case DT_IP_HI: return current.IP_HI;
        case DT_CS_LO: return current.CS_LO;
        case DT_CS_HI: return current.CS_HI;
        case DT_IP_INC8: return current.IP_INC8;
        case DT_IP_INC_LO: return current.IP_INC_LO;
        case DT_IP_INC_HI: return current.IP_INC_HI;
        case DT_ADDR_LO: return current.ADDR_LO;
        case DT_ADDR_HI: return current.ADDR_HI;
        case DT_XXX: return current.XXX;
        case DT_YYY: return current.YYY;
        case DT_IMMED8: return current.IMMED8;
        case DT_SEGREG: return current.SEGREG;
        case DT_SEG_LO: return current.SEG_LO;
        case DT_SEG_HI: return current.SEG_HI;
        case DT_DEST_STR8: return current.DEST_STR8;
        case DT_none: return current.none;
    }
}

void Decoder::printInstruction() const {
    printf("Instruction at %04X\n",current.addr);
    printf("Operation:  %02X -> %s\n",current.opcode,getOpcodeMnemonic(current.type).c_str());

    if(current.firstType != DT_none) {
        if(current.firstType == DT_modRegRM) {
            printf("First Arg Byte:\n");
            printf("    Type:  %s\n",getDataName(current.firstBytePtr).c_str());
            printf("    Value: %02X\n",getValue(current.firstType));
            printf("        Mod: %01X\n",current.mod);
            printf("        Reg: %01X\n",current.reg);
            printf("        R/M: %01X\n",current.rm);
        }else {
            printf("First Arg Byte:\n");
            printf("    Type:  %s\n",getDataName(current.firstBytePtr).c_str());
            printf("    Value: %02X\n",getValue(current.firstType));
        }
    }

    if(current.secondType != DT_none) {
        printf("Second Arg Byte:\n");
        printf("    Type:  %s\n",getDataName(current.secondBytePtr).c_str());
        printf("    Value: %02X\n",getValue(current.secondType));
    }

    if(current.thirdType != DT_none) {
        printf("Third Arg Byte:\n");
        printf("    Type:  %s\n",getDataName(current.thirdBytePtr).c_str());
        printf("    Value: %02X\n",getValue(current.thirdType));
    }

    if(current.fourthType != DT_none) {
        printf("Fourth Arg Byte:\n");
        printf("    Type:  %s\n",getDataName(current.fourthBytePtr).c_str());
        printf("    Value: %02X\n",getValue(current.fourthType));
    }

    if(current.firstType == DT_modRegRM) {
        if(current.mod == 0b01) {
            printf("DISP: %02X\n",current.DISP_LO);
        }
        if(current.mod == 0b10) {
            printf("DISP_LO: %02X\n",current.DISP_LO);
            printf("DISP_HI: %02X\n",current.DISP_HI);
        }
    }

    if(current.regA != NOREG) {
        printf("RegA: %s\n", getRegTypeName(current.regA).c_str());
    }

    if(current.regB != NOREG) {
        printf("RegB: %s\n", getRegTypeName(current.regB).c_str());
    }

}
