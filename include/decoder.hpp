/*
* Created by Alexander on 22.03.25.
 */

#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>
#include <queue>

#include "Tables.hpp"

class CPU;

struct NewInstruction {
    OpcodeType opcType;

    AddrCode ArgAddr0;
    AddrCode ArgOp0;

    AddrCode ArgAddr1;
    AddrCode ArgOp1;
};

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
    DataType firstType;

    u8* secondBytePtr;
    DataType secondType;

    u8* thirdBytePtr;
    DataType thirdType;

    u8* fourthBytePtr;
    DataType fourthType;

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


    u16* src; // either an actual address to a register in the CPU class or as a simulated address to a virtual memory location (not physical addr!)
    u16* dst; // ....
};

// Decoder class
class Decoder {
private:
    CPU* cpu;

public:
    explicit Decoder(CPU* cpuInstance) : cpu(cpuInstance) {}

    Instruction current = {0};

    void fetch(){}

    u8 readNext();
    u8 decodeModRM(u8 modRM);

    uint16_t *regReferenceByType(RegType type);


    DataType getDataType(u8* ptr);
    u8 decode(u16 ip);

    NewInstruction fillNewInstruction();

    void setRegSrcDst();


    std::string getDataName(uint8_t *ptr) const;

    uint8_t getValue(DataType dtype) const;

    void printInstruction() const;
};

#endif /* DECODER_HPP */