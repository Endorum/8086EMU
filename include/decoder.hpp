/*
* Created by Alexander on 22.03.25.
 */

#ifndef DECODER_HPP
#define DECODER_HPP

#include <cstdint>
#include <queue>

// Structure to represent a decoded instruction
struct Instruction {
    uint8_t opcode;   // Primary opcode byte
    uint8_t mod;      // MOD field
    uint8_t reg;      // REG field
    uint8_t rm;       // R/M field
    uint16_t disp;    // Displacement (if any)
    uint16_t imm;     // Immediate value (if any)
    uint8_t w;        // Word/Byte flag
    uint8_t d;        // Direction flag
};

// 6-byte prefetch queue
class Queue {
private:
    std::queue<uint8_t> queue;
    static constexpr size_t QUEUE_SIZE = 6;

public:
    void enqueue(uint8_t byte) {
        if (queue.size() >= QUEUE_SIZE) {
            queue.pop();
        }
        queue.push(byte);
    }

    uint8_t dequeue() {
        if (queue.empty()) return 0;
        uint8_t byte = queue.front();
        queue.pop();
        return byte;
    }

    bool isEmpty() const {
        return queue.empty();
    }
};

// Decoder class
class Decoder {
private:
    Queue queue;
    Instruction current = {0};

public:
    void fetch(uint8_t byte) {
        queue.enqueue(byte);
    }

    bool decode(); // To be implemented
};

#endif // DECODER_HPP
