#include "bit_manipulation.h"
#include <math.h>
#include <stdio.h>

uint32_t getBitRange(uint32_t bitString, int start, int end){
	// Indexes 32bit strings where MSB is 31 and LSB is 0
	// start: starting bit (exclusive)
	// end: last bit (inclusive)
	// Example: getBitRange(0b011010 10110..., 32, 26) = 0b...000000 011010
	
	uint32_t k = 0;
	for(int i =0; i < (start-end); i++){
		k += pow(2,i);
	}

	uint32_t mask = k << end;
	uint32_t result = (bitString & mask) >> end;

	return result;
}

uint32_t substituteByte(uint32_t bitString, uint8_t newByte, int which){
	// Substitutes a byte in a 32bit string
	// which: which byte to sub, goes from 0 to 3, 0 being most significant byte and 3 least significant
	// Example: substituteByte(0xAABBCCDD, 0xE0, 1) = 0xAAE0CCDD
	
	if(which < 0 || which > 3){
		printf("Wrong byte index given to substituteByte function, expected values range from 0 to 3 and given was '%d'\n", which);
		return bitString;
	}

	int shamt = (3-which)*8;
	uint32_t mask = 0xFF << shamt;
	uint32_t result = (bitString & (~mask)) | (newByte << shamt);

	return result;
}

uint32_t substituteHalfword(uint32_t bitString, uint16_t newHalf, int which){
	// Substitutes 16 bits in a 32bit string
	// which: which half to sub
	// Example: substituteByte(0xAABBCCDD, 0xE0, 1) = 0xAABB00E0
	
	if(which < 0 || which > 1){
		printf("Wrong half index given to substituteHalfword function, expected values range from 0 to 1 and given was '%d'\n", which);
		return bitString;
	}

	int shamt = (1-which)*16;
	uint32_t mask = 0xFFFF << shamt;
	uint32_t result = (bitString & (~mask)) | (newHalf << shamt);

	return result;
}
