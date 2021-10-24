#include "bit_manipulation.h"
#include <math.h>

uint32_t getBitRange(uint32_t bitString, int start, int end){
	// Indexes 32bit strings where MSB is 31 and LSB is 0
	// start: starting bit (exclusive)
	// end: last bit (inclusive)
	// Example: getBitRange(0b011010 10110..., 32, 26) = 0b011010 00000....
	
	uint32_t k = 0;
	for(int i =0; i < (start-end); i++){
		k += pow(2,i);
	}

	uint32_t mask = k << end;
	uint32_t result = (bitString & mask) >> end;

	return result;
}

