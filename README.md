This is a simple 8086 Emulator which can take in a binary file and runs it in a virtual machine.



# Decoding
For this emulator the first thing i want to make sure is that the decoding is accurate.
The decoding is rather complicated, the resource for this can be found in the Resources folder.

## general form (can vary)

| Byte1     | Byte2       | Byte3         | Byte4          | Byte5    | Byte6    |
|-----------|-----------  |-----------    |-----------     |----------------------
| xxxxxx xx | xx  xxx xxx | xxxxxxxx      | xxxxxxxx       | xxxxxxxx | xxxxxxxx |
| Opcode DW | MOD REG R/M | Low Disp/Data | High Disp/Data | Low Data | High Data|

### Fields
S 0: No Sign extension
1: Sign extend 8-bit immediate data to 16 bits if W = 1

W 0: Instruction operate on byte data
1: Instruction operate on word data

D 0: Instruction source is specified in REG Field
1: Instruction destination is specified in REG Field

V 0: Shift/Rotate count is one
1: Shift/Rotate count is specified in CL register

Z 0: Repeat/Loop while zero flag is clear
1: Repeat/Loop while zero flag is set


## MOD Field:
00 : Memory mode, no displacement follows (except when R/M = 110 then 16-bit displacement follows)
01 : Memory mode, 8-bit displacement follows
10 : Memory mode, 16-bit displacement follows
11 : Register mode, no displacement follows