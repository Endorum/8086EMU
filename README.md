This is a simple 8086 Emulator which can take in a binary file and runs it in a virtual machine.



# Decoding
For this emulator the first thing i want to make sure is that the decoding is accurate.
The decoding is rather complicated, the resource for this can be found in the Resources folder.

## General form (can vary)
| Byte1     | Byte2       | Byte3         | Byte4          | Byte5    | Byte6     |
|-----------|-------------|---------------|----------------|----------|-----------|
| xxxxxx xx | xx  xxx xxx | xxxxxxxx      | xxxxxxxx       | xxxxxxxx | xxxxxxxx  |
| Opcode DW | MOD REG R/M | Low Disp/Data | High Disp/Data | Low Data | High Data |

### Fields
| Field | Value | Explanation                                          |
|-------|-------|------------------------------------------------------|
| S     | 0     | No Sign extension                                    |
|       | 1     | Sign extend 8-bit immediate data to 16 bits if W = 1 |
| W     | 0     | Instruction operate on byte data                     |                    
|       | 1     | Instruction operate on word data                     |
| D     | 0     | Instruction source is specified in REG Field         |
|       | 1     | Instruction destination is specified in REG Field    |
| V     | 0     | Shift/Rotate count is one                            | 
|       | 1     | Shift/Rotate count is specified in CL register       |
| Z     | 0     | Repeat/Loop while zero flag is clear                 |
|       | 1     | Repeat/Loop while zero flag is set                   |


## MOD Field
| Code | Explanation                                                                                    |
|------|------------------------------------------------------------------------------------------------|
| 00   | Memory mode, no displacement follows (except when R/M = 110 then 16-bit displacement follows)  |
| 01   | Memory mode, 8-bit displacement follows                                                        |
| 10   | Memory mode, 16-bit displacement follows                                                       |
| 11   | Register mode, no displacement follows                                                         |


### MOD = 11 (Register Mode)
| R/M | W=0 | W=1 |
|-----|-----|-----|
| 000 | AL  | AX  |
| 001 | CL  | CX  |
| 010 | DL  | DX  |
| 011 | BL  | BX  |
| 100 | AH  | SP  |
| 101 | CH  | BP  |
| 110 | DH  | SI  |
| 111 | BH  | DI  |

mod reg rm
00  000 100

### Register index:
| Bits | 	Register (W=1, 16-bit) | Register (W=0, 8-bit) |
|------|-------------------------|-----------------------|
| 000  | AX                      | AL                    |  
| 001  | CX                      | CL                    |  
| 010  | DX                      | DL                    |  
| 011  | BX                      | BL                    |  
| 100  | SP                      | AH                    |  
| 101  | BP                      | CH                    |  
| 110  | SI                      | DH                    |  
| 111  | DI                      | BH                    |  

### MOD != 11 (Effective Address Calculation)
| R/M | MOD=00         | MOD=01         | MOD=10          |
|-----|----------------|----------------|-----------------|
| 000 | (BX)+(SI)      | (BX)+(SI) + D8 | (BX)+(SI) + D16 |
| 001 | (BX)+(DI)      | (BX)+(DI) + D8 | (BX)+(DI) + D16 |
| 010 | (BP)+(SI)      | (BP)+(SI) + D8 | (BP)+(SI) + D16 |
| 011 | (BP)+(DI)      | (BP)+(DI) + D8 | (BP)+(DI) + D16 |
| 100 | (SI)           | (SI) + D8      | (SI) + D16      |
| 101 | (DI)           | (DI) + D8      | (DI) + D16      |
| 110 | DIRECT ADDRESS | (BP) + D8      | (BP) + D16      |
| 111 | (BX)           | (BX) + D8      | (BX) + D16      |

## Segment Register Code
| Code | Register |
|------|----------|
| 00   | ES       |
| 01   | CS       |
| 10   | SS       |
| 11   | DS       |

# Queue
The 8086 has a 6 byte queue which is filled and emptied while fetching and decoding.

# Example Disassemble
| ASSEMBLY       | MACHINE CODE |
|----------------|--------------|
| MOV AX, 0F802h | B8 02 F8     |
| PUSH AX        | 50           |
| MOV CX, BX     | 8B CB        |
| MOV DX, CX     | 8B D1        |
| ADD AX, [SI]   | 03 04        |
| ADD SI, 8086h  | 81 C6 86 80  |
| JMP $-14       | EB F0        |





# General order
1. Fetch first byte into the queue
2. determine if it's a "normal" instruction or a REP etc
3. determine the length.
4. Fill the queue with the remaining bytes. the length should be stored in the queue
5. translate to a OpcodeType
6. determine the addressing mode
7. store the R/M REG, disp8/16 etc. into a struct.
8. ?

