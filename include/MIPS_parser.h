#ifndef MIPSPARSER_H
#define MIPSPARSER_H

#include <stdint.h>
#include <stddef.h>

int instructionParse(uint32_t instruction, size_t index);

// For R type instructions, since the funct field is like it's own opcode type of thing
int RParse(uint32_t instruction);

void num2reg(int reg, char* buffer);

int printInstruction(uint32_t instruction, char format, char* name, size_t index);

#endif
