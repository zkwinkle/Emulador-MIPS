#ifndef BITMANIPULATION_H
#define BITMANIPULATION_H

#include <stdbool.h>
#include <stdint.h>

uint32_t getBitRange(uint32_t bitString, int start, int end);

uint32_t substituteByte(uint32_t bitString, uint8_t newByte, int which);
uint32_t substituteHalfword(uint32_t bitString, uint16_t newHalf, int which);

#endif
