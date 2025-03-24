#include <iostream>

#include "../include/Tables.hpp"
#include "../include/CPU.hpp"


int main() {

    // u8 rom[] = {
    //     0xB8, 0x02, 0xF8,
    //     0x50,
    //     0x8B, 0xCB,
    //     0x8B, 0xD1,
    //     0x03, 0x04,
    //     0x81, 0xC6, 0x86, 0x80,
    //     0xEB, 0xF0,
    //     0xF4
    // };



    u8 rom[] = {
        0x8B, 0xCB,
        0xF4
    };

    CPU cpu;

    for(int i=0;i<sizeof(rom);i++) {
        cpu.mem.write(cpu.regs.cs, cpu.regs.ip+i, rom[i]);
    }


    cpu.step();
    // while(cpu.current.type != HLT) {
    // }




    return 0;
}
