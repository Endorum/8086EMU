//
// Created by Alexander on 22.03.25.
//

#include "../include/decoder.hpp"
#include "../include/CPU.hpp" // Now we can safely include CPU.hpp

void Decoder::fetch() {

}

uint8_t Decoder::readNext() {
    return cpu->readNext();
}




bool Decoder::requiresModRM(uint8_t opc) {
    int table[256] = {
        1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,
        1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,
        1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,
        1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,
        1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1
    };

    return table[opc] > 0;
}

uint8_t Decoder::decodeModRM(uint8_t modRM) {
    u8 Mod = (modRM & 0xC0) >> 6;
    u8 Reg = (modRM & 0x38) >> 3;
    u8 RM = (modRM & 0x07);

    current.mod = Mod;
    current.reg = Reg;
    current.rm = RM;

    if(Mod == 0b00) {
        // no disp except if RM=0b110 then Direct address
        if(RM == 0b110) {
            current.ADDR_LO = readNext();
            current.ADDR_HI = readNext();
        }

        switch (Reg) {
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

    }
    else if(Mod == 0b01) {
        // disp_LO
        current.DISP_LO = readNext();
        switch (Reg) {
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
    }
    else if(Mod == 0b10) {
        // disp LO & disp HI
        current.DISP_LO = readNext();
        current.DISP_HI = readNext();

        switch (Reg) {
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
    }
    else if(Mod == 0b11) {
        // reg mod no disp
        current.regA = NOREG;
        current.regB = NOREG;
        switch (RM) {
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
    }


    return 0;
}

uint8_t Decoder::decodeNoModRM() {

    u8 opc = current.opcode;

    u8 m = 0;

    u8 additionalBytes[256] = {
        m,m,m,m,1,2,0,0,m,m,m,m,1,2,0,0,
        m,m,m,m,1,2,0,0,m,m,m,m,1,2,0,0,
        m,m,m,m,1,2,0,0,m,m,m,m,1,2,0,0,
        m,m,m,m,1,2,0,0,m,m,m,m,1,2,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,m,
        0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,
        2,2,2,2,0,0,0,0,1,2,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,
        0,0,2,0,m,m,m,m,0,0,2,0,0,1,0,0,
        m,m,m,m,1,1,0,0,m,m,m,m,m,m,m,m,
        1,1,1,1,1,1,1,1,2,2,4,1,0,0,0,0,
        0,0,0,0,0,0,m,m,0,0,0,0,0,0,m,m
      //0 1 2 3 4 5 6 7 8 9 A B C D E F
    };

    u8 addBytes = additionalBytes[opc];

    for(int i=0;i<addBytes;i++) {
        current.addBytes[i] = readNext();
    }

    return addBytes;
}


uint8_t Decoder::decode(u16 ip) {
    current = {0};
    current.addr = ip;

    u8 bytes = 0;

    u8 opc = readNext();
    // bytes++;
    //
    current.opcode = opc;
    current.w = (opc & 0x01) > 0;
    current.d = (opc & 0x02) > 0;
    //
    // const bool hasModRM = requiresModRM(opc);
    // u8 modRM = 0xFF;
    // if(hasModRM) {
    //     modRM = readNext();
    //     bytes++;
    //     current.hasModRM = true;
    //     current.addBytes[0] = modRM;
    //     bytes += decodeModRM(modRM);
    // } else {
    //     current.hasModRM = false;
    //     bytes += decodeNoModRM();
    // }
    //
    //
    // if(opc == 0x80) {
    //     current.addBytes[1] = readNext();
    //     bytes++;
    // }
    // if(opc == 0x81) {
    //     current.addBytes[1] = readNext();
    //     current.addBytes[2] = readNext();
    //     bytes += 2;
    // }
    // if(opc == 0x82) {
    //     current.addBytes[1] = readNext();
    //     bytes += 1;
    // }
    // if(opc == 0x83) {
    //     current.addBytes[1] = readNext();
    //     bytes += 1; // signed (?)
    // }
    //

    // current.length = bytes;

    // pointer to address modes (?)

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


    // u8* fifthByte[] = {
    //     &current.XXX
    // };

    current.type = getOpcodeType(opc, current.modRegRM);

    printInstruction();

    return 0;
}



std::string interpretMod(u8 Mod) {
    if(Mod == 0b00) return "Memory mode, no displacement follows (except when R/M = 110 then 16-bit displacement follows)  ";
    if(Mod == 0b01) return "Memory mode, 8-bit displacement follows                                                        ";
    if(Mod == 0b10) return "Memory mode, 16-bit displacement follows                                                       ";
    if(Mod == 0b11) return "Register mode, no displacement follows                                                         ";
}

std::string interpretReg(u8 Reg, bool w) {
    if(Reg == 0b000) return (w) ? "AX" : "AL";
    if(Reg == 0b001) return (w) ? "CX" : "CL";
    if(Reg == 0b010) return (w) ? "DX" : "DL";
    if(Reg == 0b011) return (w) ? "BX" : "BL";
    if(Reg == 0b100) return (w) ? "SP" : "AH";
    if(Reg == 0b101) return (w) ? "BP" : "CH";
    if(Reg == 0b110) return (w) ? "SI" : "DH";
    if(Reg == 0b111) return (w) ? "DI" : "BH";
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
    if(ptr == &current.none) return "none";
}

void Decoder::printInstruction() const {
    u8 firstByte = (current.firstBytePtr != nullptr) ? *current.firstBytePtr : 0x0;
    u8 secondByte = (current.secondBytePtr != nullptr) ? *current.secondBytePtr : 0x0;
    u8 thirdByte = (current.thirdBytePtr != nullptr) ? *current.thirdBytePtr : 0x0;
    u8 fourthByte = (current.fourthBytePtr != nullptr) ? *current.fourthBytePtr : 0x0;
    printf("Instruction at %04X: %02X %02X %02X %02X\n", current.addr, current.opcode, firstByte, secondByte, thirdByte, fourthByte);
    printf("    %s\n", getOpcodeMnemonic(current.type).c_str());
    printf("    Wordmode:  %s -> %s\n",(current.w) ? "true" : "false", (current.w) ? "working with words" : "working with bytes");
    printf("    Direction: %s -> %s\n",(current.d) ? "true" : "false", (current.d) ? "Reg field specifies the source" : "Reg field specifies the destination");

    if (current.hasModRM) {
        printf("        Mod: %01X -> %s \n", current.mod, interpretMod(current.mod).c_str());
        printf("        Reg: %01X -> %s \n", current.reg, interpretReg(current.reg, current.w).c_str());
        printf("         RM: %01X -> %s \n", current.rm, interpretRM(current.rm, current.mod, current.w).c_str());
        printf("\n");
    }


    if(true) {

        if(current.firstBytePtr != &current.none) printf("%s: %02X\n",getDataName(current.firstBytePtr).c_str(),firstByte);
        if(current.secondBytePtr != &current.none) printf("%s: %02X\n",getDataName(current.secondBytePtr).c_str(),secondByte);
        if(current.thirdBytePtr != &current.none) printf("%s: %02X\n",getDataName(current.thirdBytePtr).c_str(),thirdByte);
        if(current.fourthBytePtr != &current.none) printf("%s: %02X\n",getDataName(current.fourthBytePtr).c_str(),fourthByte);

        std::string regNames[18] = {
            "",
            "AH", "AL", "AX",
            "BH", "BL", "BX",
            "CH", "CL", "CX",
            "DH", "DL", "DX",
            "SP", "BP",
            "SI", "DI",
            "IP"
        };
        if(current.regA != NOREG) printf("regA: %s\n", regNames[current.regA].c_str());
        if(current.regB != NOREG) printf("regB: %s\n", regNames[current.regB].c_str());
    }

    printf("\n");
}
