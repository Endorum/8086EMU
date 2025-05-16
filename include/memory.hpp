//
// Created by Alexander on 22.03.25.
//

#ifndef MEMORY_HPP
#define MEMORY_HPP
#include "Tables.hpp"

#include <vector>

class Memory {

public:
    explicit Memory(size_t size = 1 << 20) : mem(size, 0) {} // 1 MB default (max 8086 memory)

    u8 read(uint16_t seg, uint16_t offset) const {
        size_t addr = (seg << 4) + offset; // Convert segment:offset to linear address
        return mem[addr];
    }

    



    void write(uint16_t seg, uint16_t offset, u8 value) {
        size_t addr = (seg << 4) + offset;
        mem[addr] = value;
    }

private:
    std::vector<u8> mem;
};




#endif //MEMORY_HPP
