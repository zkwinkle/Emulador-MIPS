#ifndef MIPSMEMORY_H
#define MIPSMEMORY_H

#include <stdint.h>

static uint32_t *RB; // Register bank
static uint32_t *data; // static data
static uint32_t *heap; // heap, pointed by $gp
static uint32_t *stack; // stack, pointed by $sp
static uint32_t *MMIO; // MMIO / keyboard

void init_memory();

void load_data(); // helper function to load static data during init

void free_memory();

void storeInRegister(uint32_t word, int reg);

uint32_t getFromRegister(int reg);

#endif
