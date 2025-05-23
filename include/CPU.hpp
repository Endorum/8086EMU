//
// Created by Alexander on 22.03.25.
//

#ifndef CPU_HPP
#define CPU_HPP

#include "decoder.hpp"
#include "memory.hpp"

#include "Tables.hpp"



class Registers{

public:
    uint16_t ax = 0, bx = 0, cx = 0, dx = 0;  // General-purpose regs
    uint16_t sp = 0, bp = 0, si = 0, di = 0;  // Stack and index regs
    uint16_t cs = 0, ds = 0, es = 0, ss = 0;  // Segment regs
    uint16_t ip = 0;              // Instruction pointer
    uint16_t flags = 0;           // Flags register

    bool getFlag(u8 flag) const {
        return (flags & (1 << flag)) != 0;
    }

    void setFlag(u8 flag, bool value) {
        if (value)
            flags |= (1 << flag);
        else
            flags &= ~(1 << flag);
    }

};


class CPU {
public:


    Memory mem;
    Decoder decoder;
    Registers regs = {};
    Instruction current = {0};

    CPU();

    void step();

    void fetch();

    u8 readNext();
    u8 readAtIpWithOffset(u32 off);

    u8 readCurrent();

    void decode();



    uint16_t calculateEffectiveAddress();

    uint16_t readRegister(uint8_t idx);

    void writeRegister(uint8_t idx, uint16_t value);

    uint8_t readByte(uint16_t addr) const;

    void writeByte(uint16_t addr, uint8_t byte);

    uint16_t readUsingModRegRM();

    void _mov();
    void _add();
    void _jmp();

    void execute();


    void handleInterrupt(u8 interruptNumber);

};



#endif //CPU_HPP
