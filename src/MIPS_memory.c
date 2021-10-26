#include "MIPS_memory.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void init_memory(){
	RB = malloc(32*sizeof(uint32_t)); // 32 registros
	memset(RB, 0, 32*sizeof(uint32_t));
	RB[28] = 0x10008000; // $gp
	RB[29] = 0x7fffeffc; // $sp
	// $pc is just the file position for the text.hex, it's taken track of in text_manager

	data = malloc(1024*sizeof(uint32_t)); // 4 kB por la cantidad de líneas que vienen en el data.hex
	memset(data, 0, 1024*sizeof(uint32_t));
	load_data();

	heap = malloc(250000*sizeof(uint32_t)); // 1MB of memory
	memset(heap, 0, 250000*sizeof(uint32_t));

	stack = malloc(250000*sizeof(uint32_t)); // 1MB of memory
	memset(stack, 0, 250000*sizeof(uint32_t));

	MMIO = malloc(4*sizeof(uint32_t)); // Really only need the first 2 addresses
	memset(MMIO, 0, 4*sizeof(uint32_t));
}

void load_data(){
	FILE *dataStream = fopen("data.hex", "r");
	uint32_t word;
	size_t index = 0;
	while(fscanf(dataStream, "%X", &word) != EOF && index < 1024){
		data[index] = word;
	}
	fclose(dataStream);
}

void free_memory(){
	free(RB);
	free(data);
	free(heap);
	free(MMIO);
}

void storeInRegister(uint32_t word, int reg){
	if(reg == 0)
		return;
	RB[reg] = word;
}

uint32_t getFromRegister(int reg){
	
}
