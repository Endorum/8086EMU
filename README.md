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


Mod Reg RM
0101 1011

10 5B 0xAA

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



# Opcode Map

source: http://www.mlsite.net/8086/#oper_b

<table>
    <caption>
        <span><a name="tbl_map1">Opcode Map (Part 1)</a></span>
    </caption>
    <tr>
        <td>
        </td>
        <td class="col_header">
            0
        </td>
        <td class="col_header">
            1
        </td>
        <td class="col_header">
            2
        </td>
        <td class="col_header">
            3
        </td>
        <td class="col_header">
            4
        </td>
        <td class="col_header">
            5
        </td>
        <td class="col_header">
            6
        </td>
        <td class="col_header">
            7
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>0</p>
        </td>
        <td class="cell checker0">
            <p>ADD</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>ADD</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>ADD</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>ADD</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>ADD</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>ADD</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_ES">ES</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>POP</p>
            <p>
                <span><a href="#arg_ES">ES</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>1</p>
        </td>
        <td class="cell checker0">
            <p>ADC</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>ADC</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>ADC</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>ADC</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>ADC</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>ADC</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_SS">SS</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>POP</p>
            <p>
                <span><a href="#arg_SS">SS</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>2</p>
        </td>
        <td class="cell checker0">
            <p>AND</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>AND</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>AND</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>AND</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>AND</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>AND</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>ES:</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>DAA</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>3</p>
        </td>
        <td class="cell checker0">
            <p>XOR</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>XOR</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>XOR</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>XOR</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>XOR</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>XOR</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>SS:</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>AAA</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>4</p>
        </td>
        <td class="cell checker0">
            <p>INC</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>INC</p>
            <p>
                <span><a href="#arg_CX">CX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>INC</p>
            <p>
                <span><a href="#arg_DX">DX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>INC</p>
            <p>
                <span><a href="#arg_BX">BX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>INC</p>
            <p>
                <span><a href="#arg_SP">SP</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>INC</p>
            <p>
                <span><a href="#arg_BP">BP</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>INC</p>
            <p>
                <span><a href="#arg_SI">SI</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>INC</p>
            <p>
                <span><a href="#arg_DI">DI</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>5</p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_CX">CX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_DX">DX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_BX">BX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_SP">SP</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_BP">BP</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_SI">SI</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_DI">DI</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>6</p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>7</p>
        </td>
        <td class="cell checker0">
            <p>JO</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JNO</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>JB</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JNB</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>JZ</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JNZ</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>JBE</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JA</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>8</p>
        </td>
        <td class="cell checker0">
            <p><a href="#grp_GRP1">GRP1</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p><a href="#grp_GRP1">GRP1</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p><a href="#grp_GRP1">GRP1</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p><a href="#grp_GRP1">GRP1</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>TEST</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>TEST</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>XCHG</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>XCHG</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>9</p>
        </td>
        <td class="cell checker0">
            <p>NOP</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>XCHG</p>
            <p>
                <span><a href="#arg_CX">CX</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>XCHG</p>
            <p>
                <span><a href="#arg_DX">DX</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>XCHG</p>
            <p>
                <span><a href="#arg_BX">BX</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>XCHG</p>
            <p>
                <span><a href="#arg_SP">SP</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>XCHG</p>
            <p>
                <span><a href="#arg_BP">BP</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>XCHG</p>
            <p>
                <span><a href="#arg_SI">SI</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>XCHG</p>
            <p>
                <span><a href="#arg_DI">DI</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>A</p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_O">O</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_O">O</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#addr_O">O</a><a href="#oper_b">b</a></span>
                <span><a href="#arg_AL">AL</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#addr_O">O</a><a href="#oper_v">v</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOVSB</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOVSW</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>CMPSB</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>CMPSW</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>B</p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_CL">CL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_DL">DL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_BL">BL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_AH">AH</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_CH">CH</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_DH">DH</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_BH">BH</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>C</p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>RET</p>
            <p>
                <span><a href="#addr_I">I</a><a href="#oper_w">w</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>RET</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>LES</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_M">M</a><a href="#oper_p">p</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>LDS</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_M">M</a><a href="#oper_p">p</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>D</p>
        </td>
        <td class="cell checker0">
            <p><a href="#grp_GRP2">GRP2</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#arg_1">1</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p><a href="#grp_GRP2">GRP2</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#arg_1">1</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p><a href="#grp_GRP2">GRP2</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#arg_CL">CL</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p><a href="#grp_GRP2">GRP2</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#arg_CL">CL</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>AAM</p>
            <p>
                <span><a href="#addr_I">I</a><a href="#oper_0">0</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>AAD</p>
            <p>
                <span><a href="#addr_I">I</a><a href="#oper_0">0</a></span>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>XLAT</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>E</p>
        </td>
        <td class="cell checker0">
            <p>LOOPNZ</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>LOOPZ</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>LOOP</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JCXZ</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>IN</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>IN</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>OUT</p>
            <p>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
                <span><a href="#arg_AL">AL</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>OUT</p>
            <p>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>F</p>
        </td>
        <td class="cell checker0">
            <p>LOCK</p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>REPNZ</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>REPZ</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>HLT</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>CMC</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p><a href="#grp_GRP3a">GRP3a</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p><a href="#grp_GRP3b">GRP3b</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
    </tr>
</table>
<table>
    <caption>
        <span><a name="tbl_map2">Opcode Map (Part 2)</a></span>
    </caption>
    <tr>
        <td>
        </td>
        <td class="col_header">
            8
        </td>
        <td class="col_header">
            9
        </td>
        <td class="col_header">
            A
        </td>
        <td class="col_header">
            B
        </td>
        <td class="col_header">
            C
        </td>
        <td class="col_header">
            D
        </td>
        <td class="col_header">
            E
        </td>
        <td class="col_header">
            F
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>0</p>
        </td>
        <td class="cell checker0">
            <p>OR</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>OR</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>OR</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>OR</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>OR</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>OR</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_CS">CS</a></span>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>1</p>
        </td>
        <td class="cell checker0">
            <p>SBB</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>SBB</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>SBB</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>SBB</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>SBB</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>SBB</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
                <span><a href="#arg_DS">DS</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>POP</p>
            <p>
                <span><a href="#arg_DS">DS</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>2</p>
        </td>
        <td class="cell checker0">
            <p>SUB</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>SUB</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>SUB</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>SUB</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>SUB</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>SUB</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>CS:</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>DAS</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>3</p>
        </td>
        <td class="cell checker0">
            <p>CMP</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>CMP</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>CMP</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>CMP</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>CMP</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>CMP</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>DS:</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>AAS</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>4</p>
        </td>
        <td class="cell checker0">
            <p>DEC</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>DEC</p>
            <p>
                <span><a href="#arg_CX">CX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>DEC</p>
            <p>
                <span><a href="#arg_DX">DX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>DEC</p>
            <p>
                <span><a href="#arg_BX">BX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>DEC</p>
            <p>
                <span><a href="#arg_SP">SP</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>DEC</p>
            <p>
                <span><a href="#arg_BP">BP</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>DEC</p>
            <p>
                <span><a href="#arg_SI">SI</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>DEC</p>
            <p>
                <span><a href="#arg_DI">DI</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>5</p>
        </td>
        <td class="cell checker0">
            <p>POP</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>POP</p>
            <p>
                <span><a href="#arg_CX">CX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>POP</p>
            <p>
                <span><a href="#arg_DX">DX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>POP</p>
            <p>
                <span><a href="#arg_BX">BX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>POP</p>
            <p>
                <span><a href="#arg_SP">SP</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>POP</p>
            <p>
                <span><a href="#arg_BP">BP</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>POP</p>
            <p>
                <span><a href="#arg_SI">SI</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>POP</p>
            <p>
                <span><a href="#arg_DI">DI</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>6</p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>7</p>
        </td>
        <td class="cell checker0">
            <p>JS</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JNS</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>JPE</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JPO</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>JL</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JGE</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>JLE</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JG</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>8</p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_w">w</a></span>
                <span><a href="#addr_S">S</a><a href="#oper_w">w</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>LEA</p>
            <p>
                <span><a href="#addr_G">G</a><a href="#oper_v">v</a></span>
                <span><a href="#arg_M">M</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#addr_S">S</a><a href="#oper_w">w</a></span>
                <span><a href="#addr_E">E</a><a href="#oper_w">w</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>POP</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>9</p>
        </td>
        <td class="cell checker0">
            <p>CBW</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>CWD</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>CALL</p>
            <p>
                <span><a href="#addr_A">A</a><a href="#oper_p">p</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>WAIT</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSHF</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>POPF</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>SAHF</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>LAHF</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>A</p>
        </td>
        <td class="cell checker0">
            <p>TEST</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>TEST</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>STOSB</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>STOSW</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>LODSB</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>LODSW</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>SCASB</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>SCASW</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>B</p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_CX">CX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_DX">DX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_BX">BX</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_SP">SP</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_BP">BP</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>MOV</p>
            <p>
                <span><a href="#arg_SI">SI</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>MOV</p>
            <p>
                <span><a href="#arg_DI">DI</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>C</p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>RETF</p>
            <p>
                <span><a href="#addr_I">I</a><a href="#oper_w">w</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>RETF</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>INT</p>
            <p>
                <span><a href="#arg_3">3</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>INT</p>
            <p>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>INTO</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>IRET</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>D</p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>E</p>
        </td>
        <td class="cell checker0">
            <p>CALL</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JMP</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>JMP</p>
            <p>
                <span><a href="#addr_A">A</a><a href="#oper_p">p</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>JMP</p>
            <p>
                <span><a href="#addr_J">J</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>IN</p>
            <p>
                <span><a href="#arg_AL">AL</a></span>
                <span><a href="#arg_DX">DX</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>IN</p>
            <p>
                <span><a href="#arg_AX">AX</a></span>
                <span><a href="#arg_DX">DX</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>OUT</p>
            <p>
                <span><a href="#arg_DX">DX</a></span>
                <span><a href="#arg_AL">AL</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p>OUT</p>
            <p>
                <span><a href="#arg_DX">DX</a></span>
                <span><a href="#arg_AX">AX</a></span>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p>F</p>
        </td>
        <td class="cell checker0">
            <p>CLC</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>STC</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>CLI</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>STI</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>CLD</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>STD</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p><a href="#grp_GRP4">GRP4</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="cell checker1">
            <p><a href="#grp_GRP5">GRP5</a></p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
            </p>
        </td>
    </tr>
</table>
<table>
    <caption>
        <span><a name="tbl_ext">Opcode Map (Opcode Extensions)</a></span>
    </caption>
    <tr>
        <td>
        </td>
        <td class="col_header">
            0
        </td>
        <td class="col_header">
            1
        </td>
        <td class="col_header">
            2
        </td>
        <td class="col_header">
            3
        </td>
        <td class="col_header">
            4
        </td>
        <td class="col_header">
            5
        </td>
        <td class="col_header">
            6
        </td>
        <td class="col_header">
            7
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="grp_GRP1">GRP1</a></p>
        </td>
        <td class="cell checker0">
            <p>ADD</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>OR</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>ADC</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>SBB</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>AND</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>SUB</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>XOR</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>CMP</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="grp_GRP2">GRP2</a></p>
        </td>
        <td class="cell checker0">
            <p>ROL</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>ROR</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>RCL</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>RCR</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>SHL</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>SHR</p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>SAR</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="grp_GRP3a">GRP3a</a></p>
        </td>
        <td class="cell checker0">
            <p>TEST</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_b">b</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_b">b</a></span>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>NOT</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>NEG</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>MUL</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>IMUL</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>DIV</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>IDIV</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="grp_GRP3b">GRP3b</a></p>
        </td>
        <td class="cell checker0">
            <p>TEST</p>
            <p>
                <span><a href="#addr_E">E</a><a href="#oper_v">v</a></span>
                <span><a href="#addr_I">I</a><a href="#oper_v">v</a></span>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>NOT</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>NEG</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>MUL</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>IMUL</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>DIV</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>IDIV</p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="grp_GRP4">GRP4</a></p>
        </td>
        <td class="cell checker0">
            <p>INC</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>DEC</p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell0">
            <p></p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="grp_GRP5">GRP5</a></p>
        </td>
        <td class="cell checker0">
            <p>INC</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>DEC</p>
            <p>
            </p>
        </td>
        <td class="cell checker0">
            <p>CALL</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>CALL</p>
            <p>
                <span><a href="#addr_M">M</a><a href="#oper_p">p</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>JMP</p>
            <p>
            </p>
        </td>
        <td class="cell checker1">
            <p>JMP</p>
            <p>
                <span><a href="#addr_M">M</a><a href="#oper_p">p</a></span>
            </p>
        </td>
        <td class="cell checker0">
            <p>PUSH</p>
            <p>
            </p>
        </td>
        <td class="bad_cell1">
            <p></p>
            <p>
            </p>
        </td>
    </tr>
</table>
<table>
    <caption>
        <span><a name="tbl_addr">Argument Addressing Codes</a></span>
    </caption>
    <tr>
        <td class="row_header">
            <p><a name="addr_A">A</a></p>
        </td>
        <td class="long_cell">
            <p>Direct address.  The instruction has no ModR/M byte; the address of the operand is encoded in the instruction.  Applicable, e.g., to far JMP (opcode EA).</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="addr_E">E</a></p>
        </td>
        <td class="long_cell">
            <p>A ModR/M byte follows the opcode and specifies the operand. The operand is either a general-purpose register or a memory address.  If it is a memory address, the address is computed from a segment register and any of the following values: a base register, an index register, a displacement.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="addr_G">G</a></p>
        </td>
        <td class="long_cell">
            <p>The reg field of the ModR/M byte selects a general register.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="addr_I">I</a></p>
        </td>
        <td class="long_cell">
            <p>Immediate data.  The operand value is encoded in subsequent bytes of the instruction.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="addr_J">J</a></p>
        </td>
        <td class="long_cell">
            <p>The instruction contains a relative offset to be added to the address of the subsequent instruction.  Applicable, e.g., to short JMP (opcode EB), or LOOP.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="addr_M">M</a></p>
        </td>
        <td class="long_cell">
            <p>The ModR/M byte may refer only to memory.  Applicable, e.g., to LES and LDS.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="addr_O">O</a></p>
        </td>
        <td class="long_cell">
            <p>The instruction has no ModR/M byte; the offset of the operand is encoded as a WORD in the instruction.  Applicable, e.g., to certain MOVs (opcodes A0 through A3).</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="addr_S">S</a></p>
        </td>
        <td class="long_cell">
            <p>The reg field of the ModR/M byte selects a segment register.</p>
        </td>
    </tr>
</table>
<table>
    <caption>
        <span><a name="tbl_oper">Argument Operand Codes</a></span>
    </caption>
    <tr>
        <td class="row_header">
            <p><a name="oper_0">0</a></p>
        </td>
        <td class="long_cell">
            <p>Byte argument.  Unusual in that arguments of this type are suppressed in ASM output when they have the default value of 10 (0xA). Applicable, e.g., to AAM and AAD.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="oper_b">b</a></p>
        </td>
        <td class="long_cell">
            <p>Byte argument.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="oper_p">p</a></p>
        </td>
        <td class="long_cell">
            <p>32-bit segment:offset pointer.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="oper_w">w</a></p>
        </td>
        <td class="long_cell">
            <p>Word argument.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="oper_v">v</a></p>
        </td>
        <td class="long_cell">
            <p>Word argument.  (The 'v' code has a more complex meaning in later x86 opcode maps, from which this was derived, but here it's just a synonym for the 'w' code.)</p>
        </td>
    </tr>
</table>
<table>
    <caption>
        <span><a name="tbl_spcl">Special Argument Codes</a></span>
    </caption>
    <tr>
        <td class="row_header">
            <p><a name="arg_AL">AL</a></p>
        </td>
        <td class="long_cell">
            <p>8-bit register:  The low byte of AX</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_CL">CL</a></p>
        </td>
        <td class="long_cell">
            <p>8-bit register:  The low byte of CX</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_DL">DL</a></p>
        </td>
        <td class="long_cell">
            <p>8-bit register:  The low byte of DX</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_BL">BL</a></p>
        </td>
        <td class="long_cell">
            <p>8-bit register:  The low byte of BX</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_AH">AH</a></p>
        </td>
        <td class="long_cell">
            <p>8-bit register:  The high byte of AX</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_CH">CH</a></p>
        </td>
        <td class="long_cell">
            <p>8-bit register:  The high byte of CX</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_DH">DH</a></p>
        </td>
        <td class="long_cell">
            <p>8-bit register:  The high byte of DX</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_BH">BH</a></p>
        </td>
        <td class="long_cell">
            <p>8-bit register:  The high byte of BX</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_AX">AX</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'accumulator' register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_CX">CX</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'counter' register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_DX">DX</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'data' register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_BX">BX</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'base' register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_SP">SP</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'stack pointer' register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_BP">BP</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'base pointer' register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_SI">SI</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'source index' register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_DI">DI</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'destination index' register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_ES">ES</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'extra' segment register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_CS">CS</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'code' segment register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_SS">SS</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'stack' segment register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_DS">DS</a></p>
        </td>
        <td class="long_cell">
            <p>16-bit register:  The 'data' segment register</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_1">1</a></p>
        </td>
        <td class="long_cell">
            <p>A constant argument of 1, implicit in the opcode, and not represented elsewhere in the instruction.  This argument *is* displayed in assembly code.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_3">3</a></p>
        </td>
        <td class="long_cell">
            <p>A constant argument of 3, implicit in the opcode, and not represented elsewhere in the instruction.  This argument *is* displayed in assembly code.</p>
        </td>
    </tr>
    <tr>
        <td class="row_header">
            <p><a name="arg_M">M</a></p>
        </td>
        <td class="long_cell">
            <p>The ModR/M byte refers to a memory location, however the contents of that memory location are irrelevant; the address itself is the operand of the instruction.  Applicable, e.g., to LEA.</p>
        </td>
    </tr>
</table>

