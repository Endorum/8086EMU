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
    u8 opcode;

    u8 mod;      // MOD field
    u8 reg;      // REG field
    u8 rm;       // R/M field

    bool w; // Word/Byte flag
    bool d; // Direction flag

    OpcodeType opcType;

    AddrCode ArgAddr0;
    AddrCode ArgOp0;

    AddrCode ArgAddr1;
    AddrCode ArgOp1;

    u8 DISP_LO;
    u8 DISP_HI;

    u8 DATA_LO;
    u8 DATA_HI;

    u8 ADDR_LO;
    u8 ADDR_HI;
};

// Structure to represent a decoded instruction
struct Instruction {
    u16 addr; // address where it began

    /*
    1st byte ->
    d -> direction (0) ? instruciton source is spec. in REG field : instr dst is spec. in REG field
    w -> word -> 2bytes data/addr etc instead of 1
    */

    u8 opcode;   // Primary opcode byte
    bool w; // Word/Byte flag
    bool d; // Direction flag
    
    /*
    2nd byte -> 
    ModRM -> mod reg rm
    Data-8
    Data-LO
    IP-INC8
    DISP-LO
    ADDR-LO
    IP-INC-LO
    IP-LO
    */
    u8 secondByte;

    u8 mod;      // MOD field
    u8 reg;      // REG field
    u8 rm;       // R/M field

    u8 b2_modRegRM;
    u8 b2_DATA_8;
    u8 b2_DATA_LO;
    u8 b2_IP_INC8;
    u8 b2_DISP_LO;
    u8 b2_ADDR_LO;
    u8 b2_IP_INC_LO;
    u8 b2_IP_LO;


    /*
    3rd byte ->
    DISP-LO
    DISP-HI
    DATA-8
    DATA-LO
    DATA-HI
    DATA-SX
    ADDR-HI
    IP-INC-HI
    IP-HI
    */
    u8 byte3;
    u8 b3_DISP_LO;
    u8 b3_DISP_HI;
    u8 b3_DATA_8;
    u8 b3_DATA_LO;
    u8 b3_DATA_HI;
    u8 b3_DATA_SX;
    u8 b3_ADDR_HI;
    u8 b3_IP_INC_HI;
    u8 b3_IP_HI;

    /*
    DISP-HI
    DATA-HI
    SEG-LO
    CS-LO
    */
    u8 byte4;
    u8 b4_DISP_HI;
    u8 b4_DATA_HI;
    u8 b4_SEG_LO;
    u8 b4_CS_LO;

    /*
    DATA-LO
    DATA-8
    DATA-SX
    SEG-HI
    CS-HI
    */
    u8 byte5;
    u8 b5_DATA_LO;
    u8 b5_DATA_8;
    u8 b5_DATA_SX;
    u8 b5_SEG_HI;
    u8 b5_CS_HI;

    /*
    DATA-HI
    */
    u8 byte6;
    u8 b6_DATA_HI;

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


inline std::string toBinary(u8 byte) {
    std::string result;
    for (int i = 7; i >= 0; --i) {
        result += ((byte >> i) & 1) ? '1' : '0';
    }
    return result;
}

#endif /* DECODER_HPP */