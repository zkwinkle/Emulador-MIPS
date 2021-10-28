#ifndef MIPSINSTRUCTIONS_H
#define MIPSINSTRUCTIONS_H

#include <stdint.h>
#include <stddef.h>

// R type instructions
void add(uint32_t rd, uint32_t rs, uint32_t rt);
void add_unsigned(uint32_t rd, uint32_t rs, uint32_t rt);
void jump_register(uint32_t rs);
void set_less_than(uint32_t rd, uint32_t rs, uint32_t rt);
void shift_left_logical(uint32_t rd, uint32_t rt, uint32_t shamt);
void sub(uint32_t rd, uint32_t rs, uint32_t rt);
void xor(uint32_t rd, uint32_t rs, uint32_t rt);

// syscall
void syscall();

// I type instructions
void add_imm(uint32_t rt, uint32_t rs, uint16_t imm);
void add_imm_unsigned(uint32_t rt, uint32_t rs, uint16_t imm);

void branch_on_equal(uint32_t rs, uint32_t rt, uint16_t imm);
void branch_on_not_equal(uint32_t rs, uint32_t rt, uint16_t imm);
void branch_on_less_equal_zero(uint32_t rs, uint16_t imm);

void load_upper_imm(uint32_t rt, uint16_t imm);

void load_word(uint32_t rt, uint16_t imm, uint32_t rs);
void store_word(uint32_t rt, uint16_t imm, uint32_t rs);

void or_imm(uint32_t rt, uint32_t rs, uint16_t imm);
void set_less_than_imm(uint32_t rt, uint32_t rs, uint16_t imm);

// J type instructions
void jump(uint32_t address);
void jump_and_link(uint32_t address);

#endif
