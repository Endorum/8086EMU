/*
* Created by Alexander on 22.03.25.
 */

#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>
#include <queue>

#include "memory.hpp"
#include "Tables.hpp"

class CPU;

// Structure to represent a decoded instruction
struct Instruction {
    u16 addr; // address where it began

    uint8_t opcode;   // Primary opcode byte
    bool w; // Word/Byte flag
    bool d; // Direction flag

    u8 addBytes[6];

    bool hasModRM;
    uint8_t mod;      // MOD field
    uint8_t reg;      // REG field
    uint8_t rm;       // R/M field






    uint16_t disp;     // Displacement value (if any)
    uint16_t imm;     // Immediate value (if any)





    u16 EffAddr; // if mod != 11

    u8 length;

    OpcodeType type;
};

// // 6-byte prefetch queue
// class Queue {
// private:
//     std::queue<uint8_t> queue;
//     static constexpr size_t QUEUE_SIZE = 6;
//
// public:
//     void enqueue(uint8_t byte) {
//         if (queue.size() >= QUEUE_SIZE) {
//             queue.pop(); // Maintain FIFO behavior
//         }
//         queue.push(byte);
//     }
//
//     uint8_t dequeue() {
//         if (queue.empty()) return 0;
//         uint8_t byte = queue.front();
//         queue.pop();
//         return byte;
//     }
//
//     bool isEmpty() const {
//         return queue.empty();
//     }
//
//     size_t size() const {
//         return queue.size();
//     }
//
//     void flush() {
//         while (!queue.empty()) queue.pop();
//     }
// };


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

    uint8_t decodeNoModRM();

    uint8_t decode(u16 ip);

    void printInstruction() const;
};

#endif /* DECODER_HPP */