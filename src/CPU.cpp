//
// Created by Alexander on 22.03.25.
//

#include "../include/CPU.hpp"

#include <cassert>

CPU::CPU() : decoder(this) {} // Pass `this` to `Decoder`

uint8_t CPU::readNext() {
    uint8_t byte = mem.read(regs.cs, regs.ip); // Read from memory at CS:IP
    regs.ip++; // Increment instruction pointer
    return byte;
}

uint8_t CPU::readCurrent() {
    uint8_t byte = mem.read(regs.cs, regs.ip); // Read from memory at CS:IP
    return byte;
}

void CPU::step() {
    fetch();
    decoder.decode(regs.ip);
    current = decoder.current;
    execute();
}

void CPU::fetch() {
    u8 byte = readCurrent(); // Read next byte from memory
    //printf("%04X : %02X\n", regs.ip, byte);
}



void CPU::decode() {
    current = {0};
    decoder.decode(regs.ip);
    current = decoder.current;
}








u16 CPU::calculateEffectiveAddress() {
    u8 MOD = current.mod;
    u8 RM  = current.rm;

    //assert(MOD != 0b11 && "Not yet handled and also should not really happen");

    u16 address = 0;

    switch (RM) {
        case 0b000: address = regs.bx + regs.si; break;  // (BX)+(SI)
        case 0b001: address = regs.bx + regs.di; break;  // (BX)+(DI)
        case 0b010: address = regs.bp + regs.si; break;  // (BP)+(SI)
        case 0b011: address = regs.bp + regs.di; break;  // (BP)+(DI)
        case 0b100: address = regs.si; break;       // (SI)
        case 0b101: address = regs.di; break;       // (DI)
        case 0b110:
            if (MOD == 0b00) return current.disp;      // DIRECT ADDRESS
            else address = regs.bp; break;          // (BP) + D8/D16
        case 0b111: address = regs.bx; break;       // (BX)
        default: current.type = ERROR; break;
    }

    // Apply displacement if MOD requires it
    if (MOD == 0b01) address += static_cast<int8_t>(current.disp);   // 8-bit sign-extended displacement
    if (MOD == 0b10) address += static_cast<int16_t>(current.disp);  // 16-bit displacement

    return address;
}

u16 CPU::readRegister(u8 idx) { // idx = reg or rm
    switch (idx) {
        default: return 0xFFFF;
        case 0b000: return (current.w) ? regs.ax : (regs.ax) & 0x00FF;
        case 0b001: return (current.w) ? regs.cx : (regs.cx) & 0x00FF;
        case 0b010: return (current.w) ? regs.dx : (regs.dx) & 0x00FF;
        case 0b011: return (current.w) ? regs.bx : (regs.bx) & 0x00FF;

        case 0b100: return (current.w) ? regs.sp : ((regs.ax) & 0xFF00) >> 8;
        case 0b101: return (current.w) ? regs.bp : ((regs.cx) & 0xFF00) >> 8;
        case 0b110: return (current.w) ? regs.si : ((regs.dx) & 0xFF00) >> 8;
        case 0b111: return (current.w) ? regs.di : ((regs.bx) & 0xFF00) >> 8;
    }
}

void CPU::writeRegister(u8 idx, u16 value) {
    switch (idx) {
        default: return;
        case 0b000: (current.w) ? regs.ax = value : regs.ax |= value & 0x00FF;
        case 0b001: (current.w) ? regs.cx = value : regs.cx |= value & 0x00FF;
        case 0b010: (current.w) ? regs.dx = value : regs.dx |= value & 0x00FF;
        case 0b011: (current.w) ? regs.bx = value : regs.bx |= value & 0x00FF;

        case 0b100: (current.w) ? regs.sp = value : regs.ax |= ((value & 0xFF00) >> 8);
        case 0b101: (current.w) ? regs.bp = value : regs.cx |= ((value & 0xFF00) >> 8);
        case 0b110: (current.w) ? regs.si = value : regs.dx |= ((value & 0xFF00) >> 8);
        case 0b111: (current.w) ? regs.di = value : regs.bx |= ((value & 0xFF00) >> 8);
    }
}

// reading using seg = DS
u8 CPU::readByte(u16 addr) const {
    return mem.read(regs.ds, addr);
}

void CPU::writeByte(u16 addr, u8 byte) {
    return mem.write(regs.ds, addr, byte);
}

u16 CPU::readUsingModRegRM() {
    if(current.mod == 0b11) {

    }
}

void CPU::_mov() {
    printf("mov\n");

}

void CPU::_add() {

}

void CPU::_jmp() {

}


void CPU::execute() {


    if(current.type == MOV) {
        _mov();
    }



}





void CPU::handleInterrupt(uint8_t interruptNumber) {
    // Fetch interrupt vector from IVT (Interrupt Vector Table)
    const uint16_t vectorAddress = interruptNumber * 4;
    const uint16_t offset = mem.read(0, vectorAddress) | (mem.read(0, vectorAddress + 1) << 8);
    const uint16_t segment = mem.read(0, vectorAddress + 2) | (mem.read(0, vectorAddress + 3) << 8);

    // Simulate CALL FAR (push IP and CS onto stack, then jump)
    regs.sp -= 2;
    mem.write(regs.ss, regs.sp, regs.ip & 0xFF);
    mem.write(regs.ss, regs.sp - 1, regs.ip >> 8);

    regs.sp -= 2;
    mem.write(regs.ss, regs.sp, regs.cs & 0xFF);
    mem.write(regs.ss, regs.sp - 1, regs.cs >> 8);

    // Set new IP and CS
    regs.ip = offset;
    regs.cs = segment;
}
