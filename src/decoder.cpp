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

    u8 addedBytes = 0;

    switch (Mod) {
        default: current.mod = 0xFF;
        case 0x0: {
            if(RM == 0x6) {
                current.addBytes[1] = readNext();
                current.addBytes[2] = readNext();
                addedBytes += 2;
            }
        }break;
        case 0x1: {
            current.addBytes[1] = readNext();
            addedBytes += 1;
        }break;
        case 0x2: {
            current.addBytes[1] = readNext();
            current.addBytes[2] = readNext();
            addedBytes += 2;
        }break;
        case 0x3: {
            // no disp
        }break;

    }




    return addedBytes;
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
    bytes++;

    current.opcode = opc;
    current.w = (opc & 0x01) > 0;
    current.d = (opc & 0x02) > 0;

    const bool hasModRM = requiresModRM(opc);
    u8 modRM = 0xFF;
    if(hasModRM) {
        modRM = readNext();
        bytes++;
        current.hasModRM = true;
        current.addBytes[0] = modRM;
        bytes += decodeModRM(modRM);
    } else {
        current.hasModRM = false;
        bytes += decodeNoModRM();
    }


    if(opc == 0x80) {
        current.addBytes[1] = readNext();
        bytes++;
    }
    if(opc == 0x81) {
        current.addBytes[1] = readNext();
        current.addBytes[2] = readNext();
        bytes += 2;
    }
    if(opc == 0x82) {
        current.addBytes[1] = readNext();
        bytes += 1;
    }
    if(opc == 0x83) {
        current.addBytes[1] = readNext();
        bytes += 1; // signed (?)
    }

    current.type = getOpcodeType(opc, modRM);
    current.length = bytes;

    printInstruction();

    return 0;
}

void Decoder::printInstruction() const {
    printf("Instruction at %04X:\n", current.addr);
    printf("Length: %d\n", current.length);
    printf("    %s\n", getOpcodeMnemonic(current.type).c_str());
    printf("    W: %s\n",(current.w) ? "true" : "false");
    printf("    D: %s\n",(current.d) ? "true" : "false");

    if (current.hasModRM) {
        printf("        Mod: %01X\n", current.mod);
        printf("        Reg: %01X\n", current.reg);
        printf("         RM: %01X\n", current.rm);
        printf("\n");
    }
    for (int i = 0; i < current.length - 1; i++) {
        printf("        byte%d: %02X\n", i, current.addBytes[i]);
    }
    printf("\n");
}
