/*
* Created by Alexander on 22.03.25.
 */

#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>
#include <queue>

#include "Tables.hpp"

class CPU;

// Structure to represent a decoded instruction
struct Instruction {
    u16 addr; // address where it began

    u8 opcode;   // Primary opcode byte
    u8 addBytes[6];

    bool w; // Word/Byte flag
    bool d; // Direction flag


    bool hasModRM;
    u8 mod;      // MOD field
    u8 reg;      // REG field
    u8 rm;       // R/M field

    u16 disp;

    u8* firstBytePtr;
    u8* secondBytePtr;
    u8* thirdBytePtr;
    u8* fourthBytePtr;

    u8 modRegRM;

    u8 DATA8;
    u8 DATA_SX;
    u8 DATA_LO;
    u8 DATA_HI;

    u8 DISP_LO;
    u8 DISP_HI;

    u8 IP_LO;
    u8 IP_HI;

    u8 CS_LO;
    u8 CS_HI;

    u8 IP_INC8;
    u8 IP_INC_LO;
    u8 IP_INC_HI;

    u8 ADDR_LO;
    u8 ADDR_HI;

    u8 XXX;
    u8 YYY;

    u8 IMMED8;

    u8 SEGREG;
    u8 SEG_LO;
    u8 SEG_HI;
    u8 DEST_STR8;

    u8 none;

    u8 length;

    RegType regA;
    RegType regB;
    OpcodeType type;
};

// Decoder class
class Decoder {
private:
    CPU* cpu;

public:
    explicit Decoder(CPU* cpuInstance) : cpu(cpuInstance) {}

    Instruction current = {0};

    void fetch();

    u8 readNext();
    bool requiresModRM(u8 opc);

    u8 decodeModRM(u8 modRM);

    u8 decodeNoModRM();

    u8 decode(u16 ip);

    std::string getDataName(uint8_t *ptr) const;

    void printInstruction() const;
};

#endif /* DECODER_HPP */